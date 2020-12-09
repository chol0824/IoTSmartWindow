#include <ArduinoBearSSL.h>
#include <ArduinoECCX08.h>
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
#include "DHT.h"

#define AUTO 0          // 자동제어
#define MANUAL 1        // 수동제어

#define WINDOW_CLOSE 0  // 창문 닫힘
#define WINDOW_OPEN 1   // 창문 열림

#define DHTPIN 4        // DHT11 핀
#define DHTTYPE DHT11   // DHT11

const char ssid[]        = SECRET_SSID;         // 와이파이 ID
const char pass[]        = SECRET_PASS;         // 와이파이 비번
const char broker[]      = SECRET_BROKER;       // 엔드포인트
const char* certificate  = SECRET_CERTIFICATE;  // 인증서

WiFiClient    wifiClient;            // Used for the TCP socket connection
BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
MqttClient    mqttClient(sslClient);

DHT dht(DHTPIN, DHTTYPE); // DHT 객체 생성(4번핀, DHT11)
Servo servo;              // 서보 모터(창문 제어)

int motor_control = 5;
int led_R = 3;
int led_G = 2;
int led_B = 1;
int GP2Y1023 = 8; // 미세먼지 측정 센서

unsigned long lastMillis = 0;
unsigned long pulse = 0;

float t = 0;      // 온도 값
float h = 0;      // 습도 값
float d = 0;      // 미세먼지 농도 값
float ugm3 = 0;   // 미세먼지 농도 값

int control_state = AUTO;         // 제어 상태(자동으로 초기화)
int window_state = WINDOW_CLOSE;  // 창문 상태(닫힘으로 초기화)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(GP2Y1023, INPUT);
  pinMode(led_R, OUTPUT);
  pinMode(led_G, OUTPUT);
  pinMode(led_B, OUTPUT);
  servo.attach(motor_control);  // 서보모터 5번핀에 연결
  servo.write(0);             // 창문 닫힘 상태로 초기화
  dht.begin();                  // 온습도 센서 측정 시작

  if (!ECCX08.begin()) {
    Serial.println("No ECCX08 present!");
    while (1);
  }

  ArduinoBearSSL.onGetTime(getTime); 
  sslClient.setEccSlot(0, certificate); 
  mqttClient.onMessage(onMessageReceived);
}

void loop() {
  pulse = pulseIn(GP2Y1023, LOW, 20000);  // 미세먼지 농도 측정
  ugm3 = pulse2ugm3(pulse);   // 미세먼지 농도 측정
  d = ugm3,4;                 // 미세먼지 농도
  t = dht.readTemperature();  // 온도
  h = dht.readHumidity();     // 습도
  
  if (WiFi.status() != WL_CONNECTED) {connectWiFi();} // 와이파이 연결
  if (!mqttClient.connected()) {connectMQTT();}       // MQTT 연결
  mqttClient.poll();

  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    char payload[512];
    getDeviceStatus(payload);  
    sendMessage(payload); 
  }

  // 미세먼지 농도에 따라 LED색 변경(좋음: 초록, 보통:파랑, 나쁨:빨강)
  if(d <=30){ // 미세먼지 상태: 좋음
    //sprintf(payload,"[\"미세먼지 농도\":\"%0.2f\",\"상태\":\"%s",d,"Good");
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, HIGH);
    digitalWrite(led_B, LOW);
  }else if((30<d) && (d<81)){ // 미세먼지 상태: 보통
    //sprintf(payload,"[\"미세먼지 농도\":\"%0.2f\",\"상태\":\"%s",d,"SoSo");
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, HIGH);
  }else{  // 미세먼지 상태: 나쁨
    //sprintf(payload,"[\"미세먼지 농도\":\"%0.2f\",\"상태\":\"%s",d,"Bad");
    digitalWrite(led_R, HIGH);  
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, LOW);
  }


  // 미세먼지 농도와 온도값에 따라 창문 열기/닫기 자동 제어
  if(control_state == AUTO){
    // 창문이 닫힌 상태라면
    if(window_state == WINDOW_CLOSE){ 
      // 미세먼지 상태 좋음~보통이고 온도가 적당하면 창문 열기
      if(d<81 && (18<t && t<26)){ 
        Serial.println("창문 open");
        servo.write(80);           // 서보 모터로 창문 열기
        window_state = WINDOW_OPEN; // 창문 상태값 변경
      }
    }
    // 창문이 열린 상태라면
    else{
      // 미세먼지 상태 나쁨이거나 온도가 너무 덥거나 추우면 창문 닫기
      if(d>=81 || (t<=18 || 26<=t)){
        Serial.println("창문 close");
        servo.write(0);             // 서보 모터로 창문 닫기
        window_state = WINDOW_CLOSE;  // 창문 상태값 변경
      }
    }
  }
  delay(3000);
}


// 미세먼지 농도 측정
float pulse2ugm3(unsigned long pulse){
  float value = (pulse-1400)/14.0;
  if(value > 300){value = 0;}
  return value; // 미세먼지 농도값 반환
}

unsigned long getTime(){return WiFi.getTime();}

// 와이파이 연결
void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid); 
  Serial.print(" ");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) { 
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.println("You're connected to the network"); 
  Serial.println();
}

// MQTT 연결
void connectMQTT() {
  Serial.print("Attempting to MQTT broker: "); 
  Serial.print(broker); 
  Serial.println(" ");

  while (!mqttClient.connect(broker, 8883)) { 
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.println("You're connected to the MQTT broker"); 
  Serial.println();
  mqttClient.subscribe("$aws/things/IoTSmartWindow/shadow/update/delta"); // update/delta 주제
}

// 디바이스 상태 반환 함수
void getDeviceStatus(char* payload) { 
  t = dht.readTemperature();  // 온도
  h = dht.readHumidity();     // 습도
  d = ugm3,4;                 // 미세먼지 농도
  const char* c = (control_state == AUTO) ? "AUTO" : "MANUAL";      // 제어 상태가 자동이면 "AUTO", 수동이면 "MANUAL"
  const char* w = (window_state == WINDOW_OPEN)? "OPEN" : "CLOSE";  // 창문 상태가 열림이면 "OPEN", 닫힘이면 "CLOSE"
  sprintf(payload,"{\"state\":{\"reported\":{\"ControlState\":\"%s\", \"WindowState\":\"%s\", \"Temperature\":\"%0.2f\",\"Humidity\":\"%0.2f\",\"Dust\":\"%0.2f\"}}}",c, w, t, h, d);  // 디바이스 상태 payload에 저장
}

// 구독한 주제에 payload 게시
void sendMessage(char* payload) { 
  char TOPIC_NAME[]= "$aws/things/IoTSmartWindow/shadow/update";  // update 주제
  
  Serial.print("Publishing send message:");
  Serial.println(payload); 
  mqttClient.beginMessage(TOPIC_NAME); 
  mqttClient.print(payload);
  mqttClient.endMessage(); 
}

// 메시지 수신
void onMessageReceived(int messageSize) {
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  char buffer[512] ;
  int count=0;
  while (mqttClient.available()) { 
     buffer[count++] = (char)mqttClient.read();
  }
  buffer[count]='\0';
  Serial.println(buffer); 
  Serial.println();

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, buffer);
  JsonObject root = doc.as<JsonObject>();
  JsonObject state = root["state"];
  const char* c = state["CONTROL"];
  const char* w = state["WINDOW"];
  
  char payload[512];

  // 수동 제어
  if(strcmp(c,"MANUAL")==0){
    control_state = MANUAL; // 창문 제어 수동으로 변경
    
    // 창문 열기 
    if (strcmp(w,"OPEN")==0) {
      servo.write(80);
      window_state = WINDOW_OPEN;
      sprintf(payload,"{\"state\":{\"reported\":{\"ControlState\":\"%s\", \"WindowState\":\"%s\"}}}", "MANUAL", "OPEN");
      sendMessage(payload);
      
    } 
    // 창문 닫기
    else if (strcmp(w,"CLOSE")==0) {
      servo.write(0);
      window_state = WINDOW_CLOSE;
      sprintf(payload,"{\"state\":{\"reported\":{\"ControlState\":\"%s\", \"WindowState\":\"%s\"}}}", "MANUAL", "CLOSE");
      sendMessage(payload);
    }
  }

  // 자동 제어
  if(strcmp(c, "AUTO")==0){
    control_state = AUTO; // 창문 제어 자동으로 변경

    // 창문 열기 
    if (strcmp(w,"OPEN")==0) {
      servo.write(80);
      window_state = WINDOW_OPEN;
      sprintf(payload,"{\"state\":{\"reported\":{\"CONTROL\":\"%s\", \"WINDOW\":\"%s\"}}}", "AUTO", "OPEN");
      sendMessage(payload);
      
    } 
    // 창문 닫기
    else if (strcmp(w,"CLOSE")==0) {
      servo.write(0);
      window_state = WINDOW_CLOSE;
      sprintf(payload,"{\"state\":{\"reported\":{\"CONTROL\":\"%s\", \"WINDOW\":\"%s\"}}}", "AUTO", "CLOSE");
      sendMessage(payload);
    }
  }
}
