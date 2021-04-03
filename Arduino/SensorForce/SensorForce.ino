#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>




const char* ssid = "MIWIFI_2G_2jJ5";
const char* password = "xvFYmqRv";
int THRESHOLD = 30;
int oldNoise = -1;
const String imageServer = "http://192.168.1.139/capture"; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  Serial.println("The device started, now you can pair it with bluetooth!");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("SensorForce Ready!");
  Serial.print(WiFi.localIP());
}

void getScreenshot(){
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    http.begin(imageServer);
    int httpResponseCode = http.GET();   //Send the actual POST request
    http.end();
    Serial.println(httpResponseCode);
    if (httpResponseCode == 200){
      Serial.println("Post exitoso");
      delay(60000);
    } else {
      Serial.println("Error en GET");
    }
  }
}

void loop() {
  int noise = digitalRead(13);
  int movSensor = digitalRead(12);
  // Se dispara si no hay actividad pendiente
  Serial.println("Sensor:" + String(movSensor));
  Serial.println("Noise:" + String(noise));
  if (noise==1 && movSensor == 1){
    Serial.println("Disparar camara");
    getScreenshot();
    
  }
  delay(50);
}
