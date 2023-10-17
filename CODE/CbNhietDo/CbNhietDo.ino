#include <DHT.h>
const int DHTPIN = D4;
const int DHTTYPE = DHT11;   // Khai báo kiểu cảm biến là DHT11
//const int DHTTYPE = DHT22;   // DHT 22 
//const int DHTTYPE = DHT21;   // DHT 21 

DHT dht(DHTPIN, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến

void setup() 
{
  Serial.begin(9600);

  dht.begin(); //Khởi động cảm biến
}

void loop() 
{
  float doam = dht.readHumidity(); //Đọc độ ẩm

  float doC = dht.readTemperature(); //Đọc nhiệt độ C

  float doF = dht.readTemperature(true); //Đọc nhiệt độ F

  // Kiểm tra cảm biến có hoạt động hay không
  if (isnan(doam) || isnan(doC) || isnan(doF)) 
  {
    Serial.println("Không có giá trị trả về từ cảm biến DHT");
    return;
    delay(500);
  }

  Serial.print("Độ ẩm: ");
  Serial.print(doam);
  
  Serial.print("%  Nhiệt độ: ");
  Serial.print(doC);
  
  Serial.print("°C | ");
  Serial.print(doF);
  Serial.println("°F");

  delay(1000);
}
