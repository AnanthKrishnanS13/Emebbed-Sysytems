
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <OneWire.h>
#include <SPI.h>
#include <MFRC522.h>

#define  buzzerPin 3 // buzzer pin
#define ONE_WIRE_BUS 2  // DS18B20 data pin
#define HB_SENSOR_PIN 7 // Heartbeat sensor input
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
String para = "59 AA 34 E5";
String cof = "19 9D 34 E5";
String bet = "79 DF 7C 59";
// Initialize I2C LCD with address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
int hb = 0;

// Set up OneWire communication for DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void rf_serial()
{
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();  // Read the incoming byte
    Serial.print("Received: ");
    Serial.println(receivedChar);
    if (receivedChar == 'A') {
      Serial.println("Received 'A' - Paracetamol detected!");
    } else if (receivedChar == 'B') {
      Serial.println("Received 'B' - Cofsil detected!");
    } else if (receivedChar == 'C') {
      Serial.println("Received 'C' - Beta detected!");
    }
  }
}
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("     SMART     ");
  lcd.setCursor(0, 1);
  lcd.print(" PILL DISPENSER ");

  // Temperature sensor initialization
  sensors.begin();

  // Sensor pin modes
  pinMode(buzzerPin,OUTPUT);
  pinMode(HB_SENSOR_PIN, INPUT);  // Heartbeat sensor
  pinMode(A2, INPUT);  // ECG Lead-Off detection
  pinMode(A3, INPUT);  // ECG Lead-Off detection

  delay(2000);  // Allow sensors to stabilize
  lcd.clear();
  Serial.println("hi");
}
void check_Rfid()
{
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (i > 0) content += " ";  // Add space between bytes
    content += String(mfrc522.uid.uidByte[i], HEX);
  }

  content.toUpperCase();
  Serial.print("RFID Detected: ");
  Serial.println(content);

  if (content == para) {
    Serial.println("Paracetamol detected!");
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("Paracetamol");
    lcd.setCursor(0,1);
    lcd.print("     filling       ");
    
  } else if (content == cof) {
    Serial.println("Cofsils detected!");
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("   Cofsils     ");
    lcd.setCursor(0,1);
    lcd.print("     filling       ");
  } else if (content == bet) {
    Serial.println("Beta detected!");
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("    Beta    ");
    lcd.setCursor(0,1);
    lcd.print("    filling      ");

  } else {
    Serial.println("Access denied");
  }
  lcd.clear();
  delay(1000); 
}

void loop() {
 // Adjust delay for stable readings

  if (digitalRead(HB_SENSOR_PIN) == LOW) {
    hb = random(55, 85); // Simulated heart rate
     lcd.setCursor(9, 1);
  lcd.print("  HB:");
  lcd.print(hb);
  lcd.print("  ");  // Clear extra digits
  } 
  else {
    hb = 0;
  }
  if (hb>90)
   {
    
    digitalWrite(buzzerPin,HIGH);
    delay(2000);
    digitalWrite(buzzerPin,LOW);
   }                                         
 
  if ((digitalRead(A2) == HIGH) || (digitalRead(A3) == HIGH)) {
    Serial.println("Lead Off!");
    lcd.setCursor(0, 0);
    lcd.print("Lead Off!      ");  // Ensure old data is cleared
  } else {
    int ecgValue = analogRead(A0);
    // Serial.print("ECG=");
    Serial.println(ecgValue);
    
    lcd.setCursor(0, 0);
    lcd.print("ECG:");
    lcd.print(ecgValue);
    lcd.print("   ");  // Clear extra digits
  }
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  // Display Temperature and Heart Rate
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(tempC);
  lcd.print("C ");
   check_Rfid();
 
}
