# CentralAlarmForce
A new fork for AlarmForce. But with new features: Video streaming, photo Screenshots, noise detection in sensors
This is a project is an intent integrate IOT Solutions in Salesforce with Thingspeak as proxy. The stack selected:
* IOT Proxy:
    * Thingspeak(Rest API)
*  Hardware:
    * ESP32
         * PIR Sensor
* Software:         
    * Arduino IDE
    * Salesforce:
         * Daily Scheduled triggered flow
         * Named credential
         * External services to Thingspeak (Open API Definitions)
         * Custom Object: Sensor
         * Quick action to screen flow to manual update

##  Hardware
![Schematics](https://github.com/krukmat/CentralAlarmForce/blob/390de5740e9e64eadbeb8ba5ed9c90c8c3ecb865/Arduino/images/hardware1.png)

## General Schema
![Schematics](https://github.com/krukmat/CentralAlarmForce/blob/390de5740e9e64eadbeb8ba5ed9c90c8c3ecb865/Arduino/images/Schematics.png)

## Salesforce: 
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/email%20alert.png)
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/email_template.png)