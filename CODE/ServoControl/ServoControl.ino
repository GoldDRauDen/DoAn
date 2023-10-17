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
const char* mqttUser = "ServoControl123";
const char* mqttPassword = "01658307861Lan";
const int led = D4; 
char messageBuff[100]; // Biến dùng để lưu nội dung tin nhắn
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200); // Khởi tạo kết nối Serial để truyền dữ liệu đến máy tính
  pinMode(led, OUTPUT);
//  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, 1);     // esp8266 
  startWiFi();
  connectBroker();
  client.subscribe("Servo"); // Đăng ký nhận tin từ topic = "lights"
  client.publish("Servo", "Hello from Blocky Node"); // Gởi message = "Hello from Blocky Node" đến topic = "lights"
}
void loop() {
  if (!client.connected()) {
    connectBroker();
  }
  client.loop();
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
      delay(2000);
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
  if (message == "on") {
    digitalWrite(led, HIGH); // Bật đèn LED khi nhận được tin nhắn "on"
    // digitalWrite(LED_BUILTIN, 1); led của esp8266
    client.publish("Servo", "LED is turned ON"); // Thông báo trạng thái bật LED cho tất cả client đăng ký topic = "lights"
  }
  else if (message == "off") { // Tắt đèn LED khi nhận được tin nhắn "off"
    digitalWrite(led, LOW);
    //digitalWrite(LED_BUILTIN, 0);
    client.publish("Servo", "LED is turned OFF"); // Thông báo trạng thái tắt LED cho tất cả client đăng ký topic = "lights"
  }
}
