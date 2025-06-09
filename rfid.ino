#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

String para = "59 AA 34 E5";
String cof = "19 9D 34 E5";
String bet = "79 DF 7C 59";

void setup() {
  Serial.begin(9600);  // Initialize Serial for communication with Raspberry Pi
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready to receive from Raspberry Pi...");
}

void loop() {
  

  // *Check for RFID card*
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



  delay(2000);
}