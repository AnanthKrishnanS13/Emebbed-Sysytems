#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // Slave Select pin
#define RST_PIN 9  // Reset pin
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  Serial.println("Scan an RFID tag...");
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Create a variable to store the UID
  String tagUID = "";
  
  // Build the UID as a string
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) tagUID += "0"; // Add leading zero
    tagUID += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) tagUID += " "; // Optional: add space between bytes
  }
  tagUID.toUpperCase(); // Convert to uppercase for consistency
  
  // Print to serial and use tagUID as a variable
  Serial.print("Tag UID: ");
  Serial.println(tagUID);

  // Now you can use tagUID for comparisons, logging, etc.
  if (tagUID == "D3 FB 52 14") {
    Serial.println("Access granted!");
  } else {
    Serial.println("Access denied!");
  }

  // Halt PICC (Card)
  rfid.PICC_HaltA();
}
