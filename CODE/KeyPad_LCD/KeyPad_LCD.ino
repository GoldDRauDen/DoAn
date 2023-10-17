// Board Arduino
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> // <ESP32Servo.h>
#include <Keypad.h>
#define pin_sevor 10
int LOCK = 0;
int UNLOCK = 120;
Servo myservo;
unsigned long times = millis();
LiquidCrystal_I2C lcd(0x27, 16, 2);
char password[4];
char initial_password[] = "0000", new_password[4];
int Buzzer = 12;
int i = 0;

char key_pressed = 0;
const byte rows = 4;
const byte columns = 4;
char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte row_pins[rows] = {9, 7, 8, 6};
byte column_pins[columns] = {5, 4, 3, 2};
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {
  lcd.begin();
  myservo.attach(pin_sevor);
  lcd.backlight();
  pinMode(Buzzer, OUTPUT);
  lcd.print(" SMART HOME ");
  lcd.setCursor(3, 1);
  lcd.print("WElCOME");
  delay(3000);
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  //  initialpassword();
}

//void initialpassword() {
//  int j;
//  for (j = 0; j < 4; j++)
//    EEPROM.write(j, j + 49);
//  for (j = 0; j < 4; j++)
//    initial_password[j] = EEPROM.read(j);
//}

void loop() {
  Lock();
  Serial.begin(9600);
  key_pressed = keypad_key.getKey();
  if (key_pressed == '#')
    change();
  if (key_pressed)
  {
    password[i++] = key_pressed;
    lcd.print("*");
  }
  if (i == 4)
  {
    delay(200);
    //    for (int j = 0; j < 4; j++)
    //      initial_password[j] = EEPROM.read(j);
    if (!(strncmp(password, initial_password, 4))) {
      UnLock();
      lcd.clear();
      Serial.println(myservo.read());
      lcd.print("Pass Accepted");
      Serial.println("Pass Accepted");
      tone(Buzzer, 2500, 300);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pres >#< to");
      lcd.setCursor(0, 1);
      lcd.print("change the pass");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password:");
      lcd.setCursor(0, 1);
      i = 0;
      Lock();
    }
    else {
      Lock();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Password");
      tone(Buzzer, 2500, 2000);
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Please Try Again");
      delay(2000);
      lcd.clear();
      lcd.print("Enter Password");
      lcd.setCursor(0, 1);
      i = 0;
    }
  }
}


void change() {
  int j = 0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0, 1);
  while (j < 4) {
    char key = keypad_key.getKey();
    if (key)
    {
      new_password[j++] = key;
      lcd.print("*");
    }
    key = 0;
  }
  delay(500);
  if ((strncmp(new_password, initial_password, 4))) {
    lcd.clear();
    lcd.print("Wrong Password");
    tone(Buzzer, 2500, 300);
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(1000);
  }
  else {
    j = 0;
    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0, 1);
    while (j < 4) {
      char key = keypad_key.getKey();
      if (key)
      {
        initial_password[j] = key;
        lcd.print(key);
        EEPROM.write(j, key);
        j++;
        ;
      }
    }
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Pass Changed");
    tone(Buzzer, 2500, 300);
    delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  key_pressed = 0;
}

void Lock() {
  myservo.write(LOCK);
}

void UnLock() {
  myservo.write(UNLOCK);
}
