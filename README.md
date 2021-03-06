# CentralAlarmForce
A new fork for AlarmForce. But with new features: Video streaming, photo Screenshots, noise detection in sensors
This is a project is an intent integrate IOT Solutions in Salesforce with Thingspeak as proxy. Blynk integration to check the images through a simple ngrok configuration as well, 
The stack selected:
* IOT Proxy:
    * Thingspeak(MQTT)
*  Hardware:
    * ESP32
         * PIR Sensor
         * KY-038
    * ESP32-Cam
    * Webserver to store the screenshots
    * MQTT Client to map the events to Salesforce
* Software:         
    * Arduino IDE
    * Blynk connectivity.
    * ngrok bridge 
    * Salesforce:
         * Custom Object: Sensor

##  Hardware
![Schematics](https://github.com/krukmat/CentralAlarmForce/blob/4649af451f55758c009d65564f2bcd37ad95dbef/Arduino/images/hardware2.png)
![Schematics](https://github.com/krukmat/CentralAlarmForce/blob/390de5740e9e64eadbeb8ba5ed9c90c8c3ecb865/Arduino/images/hardware1.png)

## General Schema
![Schematics](https://github.com/krukmat/CentralAlarmForce/blob/390de5740e9e64eadbeb8ba5ed9c90c8c3ecb865/Arduino/images/Schematics.png)

## Salesforce: 
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/email%20alert.png)
![Schematics](https://github.com/krukmat/CentralAlarmForce/blob/735e282a2d621d94789de106fbdaf14bafff708b/Arduino/images/email.png)