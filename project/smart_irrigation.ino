#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 2
#define GAS_SENSOR 3
#define moisture A0
#define relay 5
LiquidCrystal_I2C lcd(0x27, 16, 4);
DHT dht(DHTPIN, DHTTYPE);
String G_condition = "";
String M_condition = "";
String URL = "";
const char* apiKey = "XLKMM63Z9NK99TAD";
void setup() {
  pinMode(GAS_SENSOR, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(moisture, INPUT);
  // --------------------------
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    SMART    ");
  lcd.setCursor(0, 1);
  lcd.print("  IRRIGATION  ");
  delay(3000);
  lcd.clear();
}

void loop() {
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int moist = analogRead(moisture);
  int gas = digitalRead(GAS_SENSOR);
  digitalWrite(relay,LOW);
  M_condition = "MOTOR_ON";
  lcd.setCursor(0, 0);
  lcd.print("   FIELD IS IN    ");
  lcd.setCursor(0, 1);
  lcd.print("   DRY CONDITION   ");
  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T : ");
  lcd.print(t);
  lcd.print("  H: ");
  lcd.print(h);
  lcd.setCursor(0, 1);
  lcd.print(G_condition);
  delay(2000);

  URL = "/update?api_key=" + String(apiKey) + "&field1=" + String(t) + "&field2=" + String(h) + "&field3=" + G_condition + "&field4=" + M_condition;
  if (gas == LOW) {
    G_condition = "LEVEL_ABNORMAL";

  } else {
    G_condition = "LEVEL__NORMAL";
  }
  //  -----------------------gas



  if (moist < 500) {
    lcd.clear();
    digitalWrite(relay,HIGH);
    M_condition = "MOTOR_OFF";
    lcd.setCursor(0, 0);
    lcd.print("   FIELD IS IN    ");
    lcd.setCursor(0, 1);
    lcd.print("   WET CONDITION   ");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T : ");
    lcd.print(t);

    // lcd.setCursor(0, 1);
    lcd.print("  H: ");
    lcd.print(h);
    lcd.setCursor(0, 1);
    lcd.print(G_condition);
    delay(2000);
  }
  Serial.print(URL);
}
