#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 4
#define gasPin 32
#define buzzerPin 25
#define RelayPin1 5
#define RelayPin2 18 
int gasread;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void ammonia() {
  if (gasread == 0) {
    lcd.setCursor(0, 1);
    lcd.print("NH3 detected        ");
     digitalWrite(RelayPin1,LOW);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("NH3 Not detected");
    digitalWrite(RelayPin1,HIGH);
  }
}
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Smart Poultry  ");
  lcd.setCursor(2, 1);
  lcd.print(" Monitoring ");
  delay(3000);
  lcd.clear();
  pinMode(gasPin, INPUT);
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {

  gasread = digitalRead(gasPin);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(2000);
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));
  Serial.println(gasread);
  ammonia();
  lcd.setCursor(0, 0);
  lcd.print(String("T:") + t);
  lcd.setCursor(9, 0);
  lcd.print(String("H:") + h);

  if (t >= 34.00) {
    digitalWrite(RelayPin2, LOW);
    digitalWrite(buzzerPin, HIGH);

  } else {
    digitalWrite(buzzerPin,LOW);
    digitalWrite(RelayPin2,HIGH);
  }
}
