#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
// Thư viện dùng để kết nối WiFi của ESP8266
#include <PubSubClient.h> // Thư viện dùng để connect, publish/subscribe MQTT
const char* ssid = "SMART"; // Tên của mạng WiFi mà bạn muốn kết nối đến
const char* password = "1l234567"; // Mật khẩu của mạng WiFi
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "LedControl123";
const char* mqttPassword = "01658307861Lan";
const char* topic = "DKLED";
#define led1 D3
#define led2 D4
#define touch1 D5
#define touch2 D6
unsigned long times = millis();
boolean btouch1 = 1; // trạng thái touch
boolean btouch2 = 1;
char messageBuff[100]; // Biến dùng để lưu nội dung tin nhắn
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); // Khởi tạo kết nối Serial để truyền dữ liệu đến máy tính
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(touch1, INPUT_PULLUP);
  pinMode(touch2, INPUT_PULLUP);
  startWiFi();
  connectBroker();
  client.subscribe(topic); // Đăng ký nhận tin từ topic = "lights"
  client.publish(topic, "Hello from Blocky Node"); // Gởi message = "Hello from Blocky Node" đến topic = "lights"
}
void loop() {
  //  if ( millis() - times >= 50) {
  //    times = millis();
  readtouch();
  if (!client.connected()) {
    connectBroker();
  }
  client.loop();
  //  }
}
void startWiFi() {
  WiFi.begin(ssid, password); // Kết nối vào mạng WiFi
  Serial.print("Connecting to ");
  Serial.print(ssid);
  // Chờ kết nối WiFi được thiết lập
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n");
  Serial.println("Connection established!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Gởi địa chỉ IP đến máy tinh
}
void connectBroker() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i;
  for (i = 0; i < length; i++) {
    messageBuff[i] = (char)payload[i];
  }
  messageBuff[i] = '\0';
  String message = String(messageBuff);
  Serial.println(message);
  if (message == "on_led1") {
    digitalWrite(led1, HIGH); // Bật đèn led1 khi nhận được tin nhắn "on"
  }
  else if (message == "off_led1") { // Tắt đèn led1 khi nhận được tin nhắn "off"
    digitalWrite(led1, LOW);
  }
  else if (message == "on_led2") {
    digitalWrite(led2, HIGH);
  }
  else if (message == "off_led2") {
    digitalWrite(led2, LOW);
  }

}

void readtouch() // đọc trạng thái của touch
{
  int ttouch1 = digitalRead(touch1);
  int ttouch2 = digitalRead(touch2);
  // toch1
  if (ttouch1 == 0) {
    if (btouch1 == 1) {
      digitalWrite(led1, LOW);
      client.publish(topic, "off_led1");
      btouch1 = 0;
    }
  }
  else {
    digitalWrite(led1, HIGH);
    client.publish(topic, "on_led1");
    btouch1 = 1;
  }
  // toch2
  if (ttouch2 == 0) {
    if (btouch2 == 1) {
      digitalWrite(led2, LOW);
      client.publish(topic, "off_led2");
      btouch2 = 0;
    }
  }
  else {
    digitalWrite(led2, HIGH);
    client.publish(topic, "on_led2");
    btouch2 = 1;
  }
}
