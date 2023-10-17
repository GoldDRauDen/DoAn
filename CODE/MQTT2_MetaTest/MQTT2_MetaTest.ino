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
// pin led
#define Relay1  D0 //GPIO 16
#define Relay2  D1 //GPIO 5
//pin touch
#define touch1 D5
#define touch2 D6
// trạng thái touch
#define Switch1 10 //SD3
#define Switch2 9  //SD2
int load1 = 1, load2 = 1, load3 = 1, load4 = 1, Power;
unsigned long times = millis();
char messageBuff[100]; // Biến dùng để lưu nội dung tin nhắn
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); // Khởi tạo kết nối Serial để truyền dữ liệu đến máy tính
  //pinMode LED
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  //pinMode touch
  pinMode(touch1, INPUT_PULLUP);
  pinMode(touch2, INPUT_PULLUP);
  startWiFi();
  connectBroker();
  client.subscribe("Relay1"); // Đăng ký nhận tin từ topic = "lights"
  client.subscribe("Relay2");
  client.publish("Relay1", "Hello from Blocky Node");
  client.publish("Relay2", "Hello from Blocky Node");
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
  // Relay1
  if (message == "on_Relay1") {
    digitalWrite(Relay1, HIGH); // Bật đèn LED khi nhận được tin nhắn "on_Relay1"
  }
  else if (message == "off_Relay1") { // Tắt đèn LED khi nhận được tin nhắn "off_Relay1"
    digitalWrite(Relay1, LOW);
  }
  // Relay2
  if (message == "on_Relay2") {
    digitalWrite(Relay2, HIGH);
  }
  else if (message == "off_Relay2") {
    digitalWrite(Relay2, LOW);
  }

}

void readtouch() // đọc trạng thái của touch
{
  if (digitalRead(Switch1) == LOW) {
    Power = 0;
    load1 != load1;
    Relay();
    client.publish("Relay1", "off");
  }
  else if (digitalRead(Switch1) == LOW) {
    Power = 0;
    load1 != load1;
    Relay();
  }
}

void Relay() {
  if (Power == 0) {
    digitalWrite(Relay1, load1);
    digitalWrite(Relay2, load2);
  } else {
    digitalWrite(Relay1, HIGH);
    digitalWrite(Relay2, HIGH);
  }
}
