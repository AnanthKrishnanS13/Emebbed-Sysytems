#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Servo.h>
Servo myservo;

#define buzzer 4
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

String tagUID = "";
const String white = "B4 5D 43 02";  // Keep in uppercase for consistency
const String blue = "D3 FB 52 14";


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  rfid.PCD_Init();
  myservo.attach(5);
  pinMode(buzzer, OUTPUT);
  myservo.write(0);
  lcd.setCursor(0, 0);
  lcd.print("    SMART      ");
  lcd.setCursor(0, 1);
  lcd.print("HOME AUTOMATION");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PLEASE ENTER");
  lcd.setCursor(0, 1);
  lcd.print("YOUR PASSKEY");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Build the UID string
  tagUID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) tagUID += "0";  // Add leading zero
    tagUID += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) tagUID += " ";
  }
  tagUID.toUpperCase();  // Convert to uppercase for consistency

  rfid.PICC_HaltA();  // Halt the card

 

  // Serial.print("Temperature: ");

  if (tagUID == blue) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME");
    lcd.setCursor(0, 1);
    lcd.print("ABINAV");

    digitalWrite(buzzer, LOW);
    myservo.write(90);
    delay(3000);
    myservo.write(0);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("AUTOMATION HUB");
lcd.setCursor(0, 1);
lcd.print("SYSTEM ONLINE");


  }

  else if (tagUID == white) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("THREAT");
    lcd.setCursor(0, 1);
    lcd.print("DETECTED");
    myservo.write(0);
    digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(buzzer, LOW);
    lcd.clear();
  } else {
    digitalWrite(buzzer, LOW);
    myservo.write(0);
  }
 
}
