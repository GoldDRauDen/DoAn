#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
// Thư viện dùng để kết nối WiFi của ESP8266
#include <PubSubClient.h> // Thư viện dùng để connect, publish/subscribe MQTT
const char* ssid = "Phan Loi"; // Tên của mạng WiFi mà bạn muốn kết nối đến
const char* password = "19082008"; // Mật khẩu của mạng WiFi
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "luanmqtt123";
const char* mqttPassword = "01658307861Lan";
const char* topic = "LED";
// pin led
#define led1  D3
#define led2  D4
//pin touch
#define touch1 D5
#define touch2 D6
// trạng thái touch
boolean btouch1 = 1;
boolean btouch2 = 1;

unsigned long times = millis();
char messageBuff[100]; // Biến dùng để lưu nội dung tin nhắn
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); // Khởi tạo kết nối Serial để truyền dữ liệu đến máy tính
  //pinMode LED
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  //pinMode touch
  pinMode(touch1, INPUT_PULLUP);
  pinMode(touch2, INPUT_PULLUP);
  startWiFi();
  connectBroker();
  client.subscribe(topic); // Đăng ký nhận tin từ topic = "lights"
  client.publish(topic, "Hello from Blocky Node"); // Gởi message = "Hello from Blocky Node" đến topic = "lights"
}
void loop() {
  if ( millis() - times >= 50) {
    times = millis();
    if (!client.connected()) {
      connectBroker();
    }
    client.loop();
    readtouch();
  }
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
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("BlockyNodeClientID")) { // Kêt nối đến broker thành công
      Serial.println("\n");
      Serial.println("MQTT connected");
    } else {
      Serial.println("\n");
      Serial.print("MQTT failed with state ");
      Serial.println(client.state());
      delay(20);
    }
  }
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
  // led1
  if (message == "on_led1") {
    digitalWrite(led1, HIGH); // Bật đèn LED khi nhận được tin nhắn "on_led1"
  }
  else if (message == "off_led1") { // Tắt đèn LED khi nhận được tin nhắn "off_led1"
    digitalWrite(led1, LOW);
  }
  // led2
  if (message == "on_led2") {
    digitalWrite(led2, HIGH);
  }
  else if (message == "off_led2") {
    digitalWrite(led2, LOW);
  }

}

void readtouch() // đọc trạng thái của touch
{
  int ttouch1 = digitalRead(touch1);// đọc trạng thái touch hiện tại
  int ttouch2 = digitalRead(touch2);

  // touch 1
  if (ttouch1 == 0) {
    if (btouch1 == 1) {
      digitalWrite(led1, LOW);
      client.publish(topic, "toff_led1");
      btouch1 = 0;
    }
  }
  else {
    digitalWrite(led1, HIGH);
    client.publish(topic, "ton_led1");
    btouch1 = 1;
  }
  // touch 2
  if (ttouch2 == 0) {
    if (btouch2 == 1) {
      digitalWrite(led2, LOW);
      client.publish(topic, "toff_led2");
      btouch2 = 0;
    }
  }
  else {
    digitalWrite(led2, HIGH);
    client.publish(topic, "ton_led2");
    btouch2 = 1;
  }
}
