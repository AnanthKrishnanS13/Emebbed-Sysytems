#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define moisture_pin 4
#define buzzer 2
const int trigPin_1 = 5;
const int echoPin_1 = 6;
long duration;
int distance;
int U1;
String bucket = "";
String waste = "";
int flag;
void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(trigPin_1, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin_1, INPUT);
  pinMode(moisture_pin, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("   SMART WASTE");
  lcd.setCursor(0, 1);
  lcd.print(" SERGATION SYSTEM");
  delay(3000);
  lcd.clear();
}
void Ultra_sonic(int a, int b) {
  // Clears the trigPin
  digitalWrite(a, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(b, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("BUCKET :");
  lcd.print(bucket);
  lcd.setCursor(0, 1);
  lcd.print(waste);
  Ultra_sonic(trigPin_1, echoPin_1);
  U1 = distance;
  if (U1 < 15) {
    bucket = "FILLED  ";

    digitalWrite(buzzer, HIGH);


  } else {
    bucket = "UNFILLED";
    digitalWrite(buzzer,LOW);

  }
  int moist = digitalRead(moisture_pin);
  if (moist == 0) {
    waste = "WET WASTE";
  } else {
    waste = "DRY WASTE";
  }
}
