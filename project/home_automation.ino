#define BLYNK_TEMPLATE_ID "TMPL3EJszTHOO"
#define BLYNK_TEMPLATE_NAME "HOME AUTOMATION"
#define BLYNK_AUTH_TOKEN "Bx63vfe6bP4ICOfKn5CZB1CnT3JUwEXy"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// #define VSENSOR_PIN 14
// float adc_voltage = 0.0;
// float in_voltage = 0.0;
// float R1 = 30000.0;
// float R2 = 7500.0;
// float ref_voltage = 5.0;
// int adc_value = 0;
#define BLYNK_PRINT Serial
char ssid[] = "MATT-4G";
char pass[] = "MATT@1234";
//#define LED_PIN 14
//#define FAN_PIN 27
//#define GND_PIN 35
int b1 = 33;
int b2 = 32;
int ledState = 0;
int fanState = 0;
int relaypin1 = 18;
int relaypin2 = 19;

BLYNK_WRITE(V0) {
  ledState = param.asInt();  // Get button value (0 or 1)
  digitalWrite(relaypin1, ledState);
  if (ledState == 1) {
    lcd.setCursor(0, 0);
    lcd.print("LIGHT ON  ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("LIGHT OFF");
  }
}
BLYNK_WRITE(V1) {
  fanState = param.asInt();  // Get button value (0 or 1)
  digitalWrite(relaypin2, fanState);
  if (fanState == 1) {
    lcd.setCursor(0, 1);
    lcd.print("FAN ON  ");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("FAN OFF");
  }
}
void setup() {

  Serial.begin(115200);
  //Serial.println("DC Voltage Test");
  //pinMode(LED_PIN, OUTPUT);
  //pinMode(FAN_PIN, OUTPUT);
  //pinMode(GND_PIN, OUTPUT);
  // digitalWrite(GND_PIN, LOW);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  digitalWrite(relaypin1, LOW);
  digitalWrite(relaypin2, LOW);
  lcd.init();
  lcd.backlight();
   Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    SMART      ");
  lcd.setCursor(0, 1);
  lcd.print("HOME AUTOMATION  ");
  delay(3000);
  lcd.setCursor(0, 0);
  lcd.print("LIGHT OFF       ");
  lcd.setCursor(0, 1);
  lcd.print("FAN OFF         ");
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
}


void loop() {
  Blynk.run();

  if (digitalRead(b1) == LOW) {
    if (ledState == 0) {
      ledState = 1;
      digitalWrite(relaypin1, HIGH);
      Blynk.virtualWrite(V0, ledState);
      lcd.setCursor(0, 0);
      lcd.print("LIGHT ON      ");
      delay(300);
    } else {
      ledState = 0;
      digitalWrite(relaypin1, LOW);
      Blynk.virtualWrite(V0, ledState);
      lcd.setCursor(0, 0);
      lcd.print("LIGHT OFF     ");
      delay(300);
    }
  }

  if (digitalRead(b2) == LOW) {
    if (fanState == 0)
    {
     fanState = 1;
     digitalWrite(relaypin2, HIGH);
     Blynk.virtualWrite(V1, fanState);
     lcd.setCursor(0, 1);
     lcd.print("FAN ON         ");
     delay(300);
    }
    else
    {
      fanState = 0;
      digitalWrite(relaypin2, LOW);
      Blynk.virtualWrite(V1, fanState);
      lcd.setCursor(0, 1);
      lcd.print("FAN OFF         ");
      delay(300);
    }
  }
}
