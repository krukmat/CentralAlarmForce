#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>




const char* ssid = "MIWIFI_5G_2jJ5_EXT";
const char* password = "xvFYmqRv";
int THRESHOLD = 30;
int oldNoise = -1;
String ipCam = "";

WiFiClient mqttIPClientWifi;
PubSubClient mqttIPClient( mqttIPClientWifi );
const char* mqtt_ip = "ioticos.org";
const int mqtt_ip_port = 1883;
const char* mqtt_ip_user = "hGR2rL1latTuCnB";
const char* mqtt_ip_password = "ASJ5c61zVvtuib7";
const char *mqtt_ip_topic = "htO9wfUxA50uzDS/output";
const char *mqtt_ip_topic_subscribe = "htO9wfUxA50uzDS/input";
char auth[] = "_Hk2RUSUh4uTDaL4468L7rrmxcds3rYn"; 
char msg[12];

void mqttCallback(char* topic, byte* payload, unsigned int length);
void mqttReconnect();

void mqttReconnect() {

  while (!mqttIPClient.connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "IOTICOS_H_W_";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (mqttIPClient.connect(clientId.c_str(),mqtt_ip_user,mqtt_ip_password)) {
      Serial.println("Conectado!");
      // Nos suscribimos
      if(mqttIPClient.subscribe(mqtt_ip_topic_subscribe)){
        Serial.println("Suscripcion ok");
      }else{
        Serial.println("fallo Suscripciión");
      }
    } else {
      Serial.print("falló :( con error -> ");
      Serial.print(mqttIPClient.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje -> " + incoming); 
  ipCam = incoming;
  String ok = "OK";
  ok.toCharArray(msg,3);
  mqttIPClient.publish(mqtt_ip_topic,msg);

}

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

  mqttIPClient.setServer(mqtt_ip, mqtt_ip_port);
  mqttIPClient.setCallback(mqttCallback);
}

void getScreenshot(){
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String imageServer = "http://"+ipCam+"/capture";
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

void mqttSendIP(){
  if (ipCam == ""){
    if (!mqttIPClient.connected()) {
      mqttReconnect();
    }  
    mqttIPClient.loop();    
  }
}

void loop() {
  mqttSendIP();
  int noise = digitalRead(13);
  int movSensor = digitalRead(12);
  if (noise==1 && movSensor == 1){
    Serial.println("Disparar camara");
    getScreenshot();
    
  }
  delay(50);
}
