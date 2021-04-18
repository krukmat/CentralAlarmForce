#include "esp_camera.h"
#include <WiFi.h>
#include "SD_MMC.h"
#include <PubSubClient.h>
#include <BlynkSimpleEsp32.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
  //

// Select camera model
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include "camera_pins.h"


WiFiClient mqttIPClientWifi;
PubSubClient mqttIPClient( mqttIPClientWifi );
const char* mqtt_ip = "ioticos.org";
const int mqtt_ip_port = 1883;
const char* mqtt_ip_user = "hGR2rL1latTuCnB";
const char* mqtt_ip_password = "ASJ5c61zVvtuib7";
const char *mqtt_ip_topic = "htO9wfUxA50uzDS/output";
const char *mqtt_ip_topic_subscribe = "htO9wfUxA50uzDS/input";
char auth[] = "_Hk2RUSUh4uTDaL4468L7rrmxcds3rYn"; 
char msg[13];


String local_address;

const char* ssid = "MIWIFI_5G_2jJ5_EXT";
const char* password = "xvFYmqRv";


boolean stopSendIp = false;

void startCameraServer();
//void generateAndSavePicture();

BLYNK_WRITE(V1)
{
  Serial.println("Boton Virtual Presionado");
  EnviarImagenBlink();
}
void EnviarImagenBlink()
{
  Serial.println("Enviando Imagen a Blink");
  long randNumber = random(3000);
  Blynk.setProperty(V2, "urls", "http://"+local_address+"/capture?id="+String(randNumber)); //ESP32 CAM 1
  delay(1000);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif
  pinMode(2, INPUT);
  pinMode(12, INPUT);

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_VGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  delay(500);
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");  
  }

  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;  
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  mqttIPClient.setServer(mqtt_ip, mqtt_ip_port);
  mqttIPClient.setCallback(mqttCallback);

  startCameraServer();

  String WiFiAddr = WiFi.localIP().toString();
  local_address = WiFiAddr;

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  
  Blynk.begin(auth, ssid, password); 

}

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
  //Serial.print("Mensaje recibido desde -> ");
  //Serial.print(topic);
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  //Serial.println("Mensaje -> " + incoming);
  stopSendIp = incoming != local_address;

}

void mqttSendIP(){
  if (!stopSendIp){
    if (!mqttIPClient.connected()) {
      mqttReconnect();
    }
  
    if (mqttIPClient.connected()){
      local_address.toCharArray(msg,14);
      //Serial.println("Enviando: "+String(msg));
      mqttIPClient.publish(mqtt_ip_topic_subscribe,msg);
      delay(300);
    }
    mqttIPClient.loop();    
  }
}

void loop() {       
  mqttSendIP();          
  Blynk.run();
}
