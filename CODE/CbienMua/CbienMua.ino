#include <Servo.h>
#define CbMua D1
#define pinServo D2
#define led D3
#define Btlen D4
#define Btxuong D5
int Lock_Pos = 0;
int UnLock_Pos = 100;
Servo myservo;
unsigned long times = millis();

void setup() {
  Serial.begin(9600);
  pinMode( led , OUTPUT);
  myservo.attach(pinServo);
  myservo.write(Lock_Pos);
  pinMode(CbMua, INPUT_PULLUP);
  pinMode(Btlen, INPUT_PULLUP);
  pinMode(Btxuong, INPUT_PULLUP);
}

void loop() {
  int trangthaiBtlen = digitalRead(Btlen);
  int trangthaiBtxuong = digitalRead(Btxuong);
  int trangthaiCbMua = digitalRead(CbMua);
  if( millis()- times > 200){
  Serial.println(trangthaiBtlen);
  Serial.println(trangthaiBtxuong);
  Serial.println(trangthaiCbMua);
  delay(200);
  
  if (trangthaiBtlen == 0 && trangthaiBtxuong != 0) {
    myservo.write(100);
    Serial.println("Mo Xao phoi Do");
    delay(500);
  }
  else if (trangthaiBtlen != 0 && trangthaiBtxuong == 0) {
    myservo.write(0);
    Serial.println("Thu Xao phoi Do");
    delay(500);
  }
  else if (trangthaiCbMua == 1) {
    myservo.write(100);
    Serial.println(" Troi Ko Co Mua mo Xao phoi Do");
    delay(500);
  }
  else if (trangthaiCbMua == 0) {
    myservo.write(0);
    Serial.println(" Troi Co Mua dong Xao phoi Do");
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
  else {
    myservo.write(0);
  }
  }
}
