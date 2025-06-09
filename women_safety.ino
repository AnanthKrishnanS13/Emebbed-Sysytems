#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#define buzzerPin 3      // buzzer pin
#define ONE_WIRE_BUS 10  // DS18B20 data pin
#define HB_SENSOR_PIN 9  // Heartbeat sensor input
#define button 7
// SDA-->gpio 21
// scl--> gpio 22
// Initialize I2C LCD with address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial GSM(4,5);  //rx,tx

int hb = 0;
int ecgValue = 0;
int tempC = 0;
String URL = "";
String prompt = "";
const char* apiKey = "XLKMM63Z9NK99TAD";
// Set up OneWire communication for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  GSM.begin(9600);
  lcd.init();  // Initialize LCD
  lcd.backlight();
  sensors.begin();
  // Sensor pin modes
  pinMode(buzzerPin, OUTPUT);
  pinMode(HB_SENSOR_PIN, INPUT);  // Heartbeat sensor
  pinMode(button, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("WOMEN SAFETY");
  lcd.setCursor(0, 1);
  lcd.print("DEVICE ACTIVE");
  delay(3000);  // Allow sensors to stabilize
  lcd.clear();
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  prompt = "SHE_IS_SAVE";
  // Read heart rate sensor (simulated with random values)
  if (digitalRead(HB_SENSOR_PIN) == LOW) {
    hb = random(55, 85);  // Simulated heart rate
  } else {
    hb = 0;
  }


  if (hb > 90) {

    digitalWrite(buzzerPin, HIGH);
    delay(2000);
    digitalWrite(buzzerPin, LOW);
  }

  if (button != LOW) {
    delay(10000);
    if (button != LOW) {
      prompt = "SHE_IS_UNSAVE";
      digitalWrite(buzzerPin, HIGH);
      delay(2000);
      digitalWrite(buzzerPin, LOW);
      GSM.println("AT+CMGF=1");
      delay(500);
      GSM.println("AT+CMGF=\"+919876543210\"\r");
      delay(500);
      GSM.println("Emergency detected! I need help. Please track my location.");
      GSM.println((char)26);
      delay(500);
    }
  }
  if(button == LOW )
  {
    GSM.println((char)27);
  }

  URL = "/update?api_key=" + String(apiKey) + "&field1=" + String(tempC) + "&field2=" + String(hb);
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(tempC);
  lcd.print("C ");
  lcd.print("  HB:");
  lcd.print(hb);
  lcd.print("  ");  // Clear extra digits
  
  lcd.setCursor(1, 0);
  lcd.print(prompt);
  delay(200);  // Adjust delay for stable readings
  Serial.println(URL);
}