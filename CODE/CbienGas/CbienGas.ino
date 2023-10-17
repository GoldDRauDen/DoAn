#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#define led D2
int buzzer = D3;
int mucCanhbao = 350;
int mq2_value;
int button = 0;
boolean buttonState = HIGH;
boolean runMode = 1; //Bật/tắt chế độ cảnh báo
boolean canhbaoState = 0;

void setup()
{
  delay(100);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  Serial.begin(115200);
  digitalWrite(buzzer, LOW); //Tắt buzzer
  digitalWrite(led, LOW);
  mq2_value = analogRead(A0);
  Serial.println(mq2_value);
  delay(500);
  if(mq2_value > mucCanhbao) {
    Serial.println(mq2_value);
    Serial.println("Canh Báo Khí Gas Cao");
    tone(buzzer, 2500, 3000);
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}
