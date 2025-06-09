#include <LiquidCrystal_I2C.h>

#include <Servo.h>

Servo myservo;

#define IR 4
const int trigPin_1 = 5;
const int echoPin_1 = 6;
const int trigPin_2 = 9;
const int echoPin_2 = 10;
LiquidCrystal_I2C lcd(0x27, 16, 2);
String slot1 = "";
String slot2 = "";
long duration;
int distance;
int U1, U2;
int flag1, flag2, flag3;

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
void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  Serial.begin(9600);
  myservo.attach(11);
  myservo.write(0);
  // ?----------
  pinMode(trigPin_1, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin_1, INPUT);   // Sets the echoPin as an Input     // Calculate Rs
  pinMode(trigPin_2, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin_2, INPUT);
  pinMode(IR, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("    SMART PARKING");
  lcd.setCursor(0, 1);
  lcd.print("      SYSTEM");
  delay(3000);
  lcd.clear();
}

void loop() {
 
  Ultra_sonic(trigPin_1, echoPin_1);
  U1 = distance;
  // Ultra_sonic(trigPin_2, echoPin_2);
  // U2 = distance;
  delay(100);
  Serial.print(U1);
  Serial.print("     ");
    Serial.println(U2);

  // Serial.println(digitalRead(IR));
  if (U1 < 15) {
    slot1 = "OCCUPIED  ";
    flag1 = 1;
  }
   else
  {
    slot1 = "VACANT  ";
  }
  if (U2 < 15) {
    slot2 = "OCCUPIED  ";
    flag2 = 1;
  }
  else
  {
    slot2 = "VACANT  ";
  }
  if ((U1 < 15) && (U2 < 15)) {
    slot1 = "OCCUPIED    ";
    slot2 = "OCCUPIED    ";
    flag3 = 1;
  }
  else 
  {
    flag3= 0;
  }




  if ((digitalRead(IR)==0) && (flag3 ==0)) {

    myservo.write(90);
    delay(3000);
    myservo.write(0);
  }


  lcd.setCursor(0, 0);
  lcd.print("SLOT 1 : ");
  lcd.print(slot1);
  lcd.setCursor(0, 1);
  lcd.print("SLOT 2 : ");
  lcd.print(slot2);
}
