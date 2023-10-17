#define touch1 D5
#define led1 D3
#define touch2 D6
#define led2 D4
boolean btouch1 = 1 ;
boolean btouch2 = 1 ;
void setup() {
  Serial.begin(9600);
  pinMode( touch1, INPUT_PULLUP);
  pinMode( led1, OUTPUT);
  pinMode( touch2, INPUT_PULLUP);
  pinMode( led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

void loop() {
  int ttTouch1 = digitalRead(touch1);
  int ttTouch2 = digitalRead(touch2);
  if (ttTouch1 == 0) {
    if (btouch1 == 1) {
      digitalWrite(led1, LOW);
      Serial.println("OFF Led1");
      btouch1 = 0;
    }
  }
  else {
    digitalWrite(led1, HIGH);
    Serial.println("ON Led1");
    btouch1 = 1;
  }

   if (ttTouch2 == 0) {
    if (btouch2 == 1) {
      digitalWrite(led2, LOW);
      Serial.println("OFF Led2");
      btouch2 = 0;
    }
  }
  else {
    digitalWrite(led2, HIGH);
    Serial.println("ON Led2");
    btouch2 = 1;
  }
}
