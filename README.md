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
# Test device
```bash
platformio run -t upload && platformio device monitor -b 115200
```

You need to define the following properties in your build environment variable (`PLATFORMIO_BUILD_FLAGS`) or in `platformio.ini`
- `SSID`
- `PASSWORD`
- `THINGSPEAK_HOST`
- `THINGSPEAK_API_KEY`

Example:
```bash
export PLATFORMIO_BUILD_FLAGS=-DSSID=myssid -DPASSWORD=mypass -DTHINGSPEAK_HOST=myhost -DTHINGSPEAK_API_KEY=mykey
```
