#include <ESP8266WiFi.h>
#include <DHT.h>
#define str(macro) #macro
#define to_str(macro) str(macro)
#if defined SSID && defined PASSWORD && defined THINGSPEAK_HOST && defined THINGSPEAK_API_KEY
#define SSID_value to_str(SSID) 
#define PASSWORD_value to_str(PASSWORD)
#define THINGSPEAK_HOST_value to_str(THINGSPEAK_HOST)
#define THINGSPEAK_API_KEY_value to_str(THINGSPEAK_API_KEY)
#else
#error You need to define SSID PASSWORD THINGSPEAK_HOST THINGSPEAK_API_KEY
#endif
/***************************
 * Begin Settings
 **************************/


const char* ssid      = SSID_value;
const char* password  = PASSWORD_value;
const char* host      = THINGSPEAK_HOST_value;
const char* key       = THINGSPEAK_API_KEY_value;

// DHT Settings
#define DHTPIN D6     // what digital pin we're connected to. If you are not using NodeMCU change D6 to real pin
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// Initialize the temperature/ humidity sensor
DHT dht(DHTPIN, DHTTYPE);

const int UPDATE_INTERVAL_SECONDS = 300;

typedef struct data {
  float humidity;
  float temperature;
  uint16_t voltage;
} DATA;

/***************************
 * End Settings
 **************************/

void setup() {
  Serial.begin(115200);
  delay(10);
  
  connectWifi();
  postData(readData());

  Serial.println();
  Serial.println("closing connection");
  ESP.deepSleep(1 * 60 * 1000000, WAKE_RF_DEFAULT); //, WAKE_RF_DISABLED);  //sleep 1 second
}

void loop() {
}

void connectWifi() {
  //We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("connecting to ");
  Serial.println(host);
}

DATA readData() {
  DATA data;
  dht.begin();
  while (true) {
    data.humidity = dht.readHumidity();
    data.temperature = dht.readTemperature();
    data.voltage = ESP.getVcc();   
    //if data were not properly read, wait for 2 seconds and repeat
    if ( !isnan(data.humidity) && !isnan(data.temperature) ) break;
    Serial.println("");
    Serial.println("Reading failed, repeating");
    delay(2000);
  }
  return data;
}

void postData(DATA data) {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/update?api_key=";
  url += key;
  url += "&field1=";
  url += String(data.temperature);
  url += "&field2=";
  url += String(data.humidity);
  url += "&field3=";
  url += String(data.voltage);

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" + 
      "Connection: close\r\n\r\n");
  delay(10);
  while(!client.available()){
    delay(100);
    Serial.print(".");
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}
