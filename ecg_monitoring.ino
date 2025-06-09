#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define  buzzerPin 3 // buzzer pin
#define ONE_WIRE_BUS 27  // DS18B20 data pin
#define HB_SENSOR_PIN 32 // Heartbeat sensor input
// SDA-->gpio 21
// scl--> gpio 22
// Initialize I2C LCD with address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
int hb = 0;

// Set up OneWire communication for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("     Health    ");
  lcd.setCursor(0, 1);
  lcd.print("   Monitoring  ");

  // Temperature sensor initialization
  sensors.begin();

  // Sensor pin modes
  pinMode(buzzerPin,OUTPUT);
  pinMode(HB_SENSOR_PIN, INPUT);  // Heartbeat sensor
  pinMode(16, INPUT);  // ECG Lead-Off detection
  pinMode(17, INPUT);  // ECG Lead-Off detection

  delay(3000);  // Allow sensors to stabilize
  lcd.clear();
}

void loop() {
  // Read temperature
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Read heart rate sensor (simulated with random values)
  if (digitalRead(HB_SENSOR_PIN) == LOW) {
    hb = random(55, 85); // Simulated heart rate
  } else {
    hb = 0;
  }
  
  // Serial.print("HB=");
  // Serial.println(hb);

  // ECG Lead-Off Detection
  if ((digitalRead(16) == HIGH) || (digitalRead(17) == HIGH)) {
    Serial.println("Lead Off!");
    lcd.setCursor(0, 0);
    lcd.print("Lead Off!      ");  // Ensure old data is cleared
  } else {
    int ecgValue = analogRead(35);
    // Serial.print("ECG=");
    Serial.println(ecgValue);
    
    lcd.setCursor(0, 0);
    lcd.print("ECG:");
    lcd.print(ecgValue);
    lcd.print("   ");  // Clear extra digits
  }

  // Display Temperature and Heart Rate
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(tempC);
  lcd.print("C ");

   if (hb>90)
   {
    
    digitalWrite(buzzerPin,HIGH);
    delay(2000);
    digitalWrite(buzzerPin,LOW);
   }                                         
  lcd.setCursor(9, 1);
  lcd.print("  HB:");
  lcd.print(hb);
  lcd.print("  ");  // Clear extra digits

  delay(200);  // Adjust delay for stable readings
}
