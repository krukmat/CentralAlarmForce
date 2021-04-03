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
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/AlarmForce%20-%20Schematics.png)
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/Hardware.png)

## General Schema
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/Untitled%20Diagram.png)

## Salesforce: 
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/email%20alert.png)
![Schematics](https://raw.githubusercontent.com/krukmat/AlarmForce-Demo/mqtt/images/email_template.png)