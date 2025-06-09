
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
// #include "BluetoothSerial.h"
// #include <BLEServer.h>
#define moisture 8
#define DHTPIN 9
#define DHTTYPE DHT11
#define m_pin1 2
#define m_pin2 3
#define m_pin3 4
#define m_pin4 5
#define relay 6
String moist_condition = "";
char received;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int moist_Con;
// BluetoothSerial SerialBT;
// BLEServer* pServer;
void setup() {
  dht.begin();
  Serial.begin(9600);
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  pinMode(m_pin1, OUTPUT);
  pinMode(m_pin2, OUTPUT);
  pinMode(m_pin3, OUTPUT);
  pinMode(m_pin4, OUTPUT);
  pinMode(relay, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("SMART IRRIGATION  ");
  lcd.setCursor(0, 1);
  lcd.print("   ROBOT  ");
  delay(3000);
  lcd.clear();
}

void loop() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print(" H:");
  lcd.print(h);
  lcd.setCursor(0, 1);
  lcd.print(moist_condition);
  int moist = digitalRead(moisture);
  if (moist == 0) {
    moist_condition = "WET CONDITION";
    digitalWrite(relay, LOW);
    moist_Con = 0;

  } else {
    moist_condition = "DRY CONDITION";
    digitalWrite(relay, HIGH);
    moist_Con = 1;
  }
  if (Serial.available() > 0) {
    received = Serial.read();
    Serial.print("Received: ");
    Serial.println(received);

    switch (received) {
      case 'F':
        Forward();
        // Serial.println("FORWARD");
        break;
      case 'B':
        Backward();
        // Serial.println("BACKWARD");
        break;
      case 'L':
        Left();
        // Serial.println("LEFT");
        break;
      case 'R':
        Right();
        // Serial.println("RIGHT");
        break;
      case 'S':
        Stop();
        // Serial.println("STOP");
        break;
    }
  }
  Serial.print("*");
  Serial.print(t);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.print(moist_Con);
  Serial.println("#");
}
void Forward() {
  digitalWrite(m_pin1, HIGH);
  digitalWrite(m_pin2, LOW);
  digitalWrite(m_pin3, HIGH);
  digitalWrite(m_pin4, LOW);
}
void Backward() {
  digitalWrite(m_pin1, LOW);
  digitalWrite(m_pin2, HIGH);
  digitalWrite(m_pin3, LOW);
  digitalWrite(m_pin4, HIGH);
}
void Right() {
  digitalWrite(m_pin1, HIGH);
  digitalWrite(m_pin2, LOW);
  digitalWrite(m_pin3, LOW);
  digitalWrite(m_pin4, HIGH);
}
void Left() {
  digitalWrite(m_pin1, LOW);
  digitalWrite(m_pin2, HIGH);
  digitalWrite(m_pin3, HIGH);
  digitalWrite(m_pin4, LOW);
}
void Stop() {
  digitalWrite(m_pin1, LOW);
  digitalWrite(m_pin2, LOW);
  digitalWrite(m_pin3, LOW);
  digitalWrite(m_pin4, LOW);
}
