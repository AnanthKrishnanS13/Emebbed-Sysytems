void setup() {
  Serial.begin(9600);     // Default serial monitor
  // Serial.swap();          // Swap UART to use GPIO13 (D7) as RX
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.println("Received: " + data);
  }
  
}
