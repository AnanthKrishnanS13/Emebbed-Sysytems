#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

int trig1 = 2;
int echo1 = 3;

int trig2 = 4;
int echo2 = 5;

int trig3 = 6;
int echo3 = 7;
int count = 0;
int ir = 11;

int distance1, distance2, distance3, distance;
int threshold = 6;
long duration;
String url = "", field1data, field2data, field3data;
String field;

int flag = 0;

void setup() {

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("    VEHICLE    ");
  lcd.setCursor(0, 1);
  lcd.print("    PARKING     ");
  delay(3000);
  lcd.clear();

  servo.attach(9);
  servo.write(180);

  pinMode(ir, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo3, INPUT);
}

void loop() {

  ultrasonicRead(trig1, echo1);
  distance1 = distance;
  ultrasonicRead(trig2, echo2);
  distance2 = distance;
  ultrasonicRead(trig3, echo3);
  distance3 = distance;

//   Serial.print("Slot1: ");
//   Serial.print(distance1);
//   Serial.print(" cm\t");
//   Serial.print("Slot2: ");
//   Serial.print(distance2);
//   Serial.print(" cm\t");
//   Serial.print("Slot3: ");
//   Serial.print(distance3);
//   Serial.println(" cm");

  int freeSlots = 0;



  if (distance1 > threshold) freeSlots++;
  if (distance2 > threshold) freeSlots++;
  if (distance3 > threshold) freeSlots++;
  lcd.clear();
  if (freeSlots > 0) {
    lcd.setCursor(0, 0);
    lcd.print("FREE SLOTS:");
    lcd.setCursor(0, 1);
    lcd.print(freeSlots);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("NO SLOT");
    lcd.setCursor(0, 1);
    lcd.print("AVAILABLE");
  }

  if (digitalRead(ir) == LOW && freeSlots > 0) {
    servo.write(80);
  } else {
    servo.write(180);
  }
  delay(500);
  count++;
  if (count > 10) {
    url = "/update?api_key=30R0962LKPK2V3SS";
    url += "&field1=";
    url += freeSlots;
    Serial.println(url);
    count=0;
  }
}

void ultrasonicRead(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  //   Serial.println(distance);
  //delay(300);
  //return distance;
}
