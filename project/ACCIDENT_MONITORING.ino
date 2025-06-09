#include <Wire.h>
#include <MPU6050.h>
#include <LiquidCrystal_I2C.h>

MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 0x27 is I2C address, 16x2 LCD

#define KEY 7
#define M1 3
#define M2 4
#define VIBRATION 2

int keyState;
int flag = 0;
int newflag = 0;

String VEHICLE = "OFF";
double lat1 = 8.207685;
double lon1 = 77.399091;
String ACC = "";
void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();

  pinMode(KEY, INPUT_PULLUP);
  pinMode(VIBRATION, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  // if (mpu.testConnection()) {
  //   Serial.println("MPU6050 connection successful");
  // } else {
  //   Serial.println("MPU6050 connection failed");
  // }

  lcd.setCursor(0, 0);
  lcd.print(" IoT-Based     ");
  lcd.setCursor(0, 1);
  lcd.print("Accident Detect");
  delay(2000);
  lcd.clear();
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  keyState = digitalRead(KEY);

  // Debugging output
//  Serial.print("AX: ");
//  Serial.print(ax);
//  Serial.print(" AY: ");
//  Serial.print(ay);
//  Serial.print(" AZ: ");
//  Serial.print(az);
//  Serial.print(" | Vibration: ");
//  Serial.println(digitalRead(VIBRATION));


  if (keyState == LOW) {

    if ((ax > -10000 && ax < 10000 && ay > -10000 && ay < 10000) && digitalRead(VIBRATION) == LOW) {

      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("VEHICLE RUNNING ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      ACC = "VEHICLE_RUNNING";

    } 
    else {

      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      lcd.setCursor(0, 0);
      lcd.print("ACCIDENT DETECT");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      ACC = "ACCIDENT_DETECTED";
      delay(500);
    }
  }


  else {
    digitalWrite(M1, LOW);
    digitalWrite(M2, LOW);
    lcd.setCursor(0, 0);
    lcd.print("TURN ON ...    ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  delay(500);
  Serial.print("*");
  Serial.print(lat1,6);
  Serial.print(",");
  Serial.print(lon1,6);
  
  Serial.println("#");
}
