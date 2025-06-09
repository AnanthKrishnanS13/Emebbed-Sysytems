void setup() {
  Serial.begin(9600);  // Start serial at 9600 baud
}

void loop() {
  Serial.println("Hello from Uno \n");  // Send message
  delay(1000);
  Serial.println("Hello from Uno1 \n");
}
