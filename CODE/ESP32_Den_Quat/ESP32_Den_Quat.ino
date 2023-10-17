#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h> // Thư viện dùng để connect, publish/subscribe MQTT

const char* ssid = "Smart"; // Tên của mạng WiFi mà bạn muốn kết nối đến
const char* password = "1l2345678"; // Mật khẩu của mạng WiFi
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "luanmqtt123";
const char* mqttPassword = "01658307861Lan";
// PIN_LED
#define led_truoc
#define led_khach
#define led_ngu1
#define led_ngu2
#define led_bep
#define led_sau
// PIN_QUAT
#define fan_khach
#define fan_ngu1
#define fan_ngu2
#define fan_bep
// PIN_TOUCH_LED
#define touch_led_truoc
#define touch_led_khach
#define touch_led_ngu1
#define touch_led_ngu2
#define touch_led_bep
#define touch_led_sau
// PIN_TOUCH
#define touch_fan_khach
#define touch_fan_ngu1
#define touch_fan_ngu2
#define touch_fan_bep

char messageBuff[100]; // Biến dùng để lưu nội dung tin nhắn
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); // Khởi tạo kết nối Serial để truyền dữ liệu đến máy tính
  //pinMode_led_fan
  pinMode(led_truoc, OUTPUT);
  pinMode(led_khach, OUTPUT);
  pinMode(led_ngu1, OUTPUT);
  pinMode(led_ngu2, OUTPUT);
  pinMode(led_bep, OUTPUT);
  pinMode(led_sau, OUTPUT);
  pinMode(fan_khach, OUTPUT);
  pinMode(fan_ngu1, OUTPUT);
  pinMode(fan_ngu2, OUTPUT);
  pinMode(fan_bep, OUTPUT);
  //pinMode_touch
  pinMode(touch_led_truoc, INPUT_PULLUP);
  pinMode(touch_led_khach, INPUT_PULLUP);
  pinMode(touch_led_ngu1, INPUT_PULLUP);
  pinMode(touch_led_ngu2, INPUT_PULLUP);
  pinMode(touch_led_bep, INPUT_PULLUP);
  pinMode(touch_led_sau, INPUT_PULLUP);
  pinMode(touch_fan_khach, INPUT_PULLUP);
  pinMode(touch_fan_ngu1, INPUT_PULLUP);
  pinMode(touch_fan_ngu2, INPUT_PULLUP);
  pinMode(touch_fan_bep, INPUT_PULLUP);

  startWiFi();
  connectBroker();
  client.subscribe("LED"); // Đăng ký nhận tin từ topic = "LED"
  client.publish("LED", "Điều Khiển LED OKE"); // Gởi message = "Điều Khiển LED OKE" đến topic = "LED"
  client.subscribe("FAN");
  client.publish("FAN", "Điều Khiển FAN OKE");
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
  //LED trước
  if (message == "led_truoc_on") {
    digitalWrite(led_truoc, HIGH); // Bật đèn LED  trước khi nhận được tin nhắn "led_ngu1_on"
    client.publish("LED", "LED trước mở"); // Thông báo trạng thái bật LED trước cho tất cả client đăng ký topic = "LED"
  }
  else if (message == "led_truoc_off") { // Tắt đèn LED trước khi nhận được tin nhắn "led_truoc_off"
    digitalWrite(led_truoc, LOW);
    client.publish("LED", "LED trước tắt"); // Thông báo trạng thái tắt LED trước cho tất cả client đăng ký topic = "LED"
  }
  //LED KHÁCH
  if (message == "led_khach_on") {
    digitalWrite(led_khach, HIGH);
    client.publish("LED", "LED khách mở");
  }
  else if (message == "led_khach_off") {
    digitalWrite(led_khach, LOW);
    client.publish("LED", "LED khách tắt");
  }
  //LED_NGU1
  if (message == "led_ngu1_on") {
    digitalWrite(led_ngu1, HIGH);
    client.publish("LED", "LED ngu1 mở");
  }
  else if (message == "led_ngu1_off") {
    digitalWrite(led_ngu1, LOW);
    client.publish("LED", "LED ngu1 tắt");
  }
  //LED_NGU2
  if (message == "led_ngu2_on") {
    digitalWrite(led_ngu2, HIGH);
    client.publish("LED", "LED ngu2 mở");
  }
  else if (message == "led_ngu2_off") {
    digitalWrite(led_ngu2, LOW);
    client.publish("LED", "LED ngu2 tắt");
  }
  //LED_BEP
  if (message == "led_bep_on") {
    digitalWrite(led_bep, HIGH);
    client.publish("LED", "LED bep mở");
  }
  else if (message == "led_bep_off") {
    digitalWrite(led_bep, LOW);
    client.publish("LED", "LED bep tắt");
  }


}
