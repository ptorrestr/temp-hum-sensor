# Temp-Humidity Sensor

Get temperatue and humidity (DHT11) and put it on a server using nodemcu v2

# Compile
```bash
platformio run
```

# Upload into device
```bash
platformio run -t upload
```

You need to define the following properties in your build environment variabl (PLATFORMIO\_BUILD\_FLAGS)
- SSID
- PASSWORD
- THINGSPEAK\_HOST
- THINGSPEAK\_API\_KEY

Example:
```bash
export PLATFORMIO_BUILD_FLAGS=-DSSID=myssid -DPASSWORD=mypass -DTHINGSPEAK_HOST=myhost -DTHINGSPEAK_API_KEY=mykey
```
