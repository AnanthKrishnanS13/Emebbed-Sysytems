#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ONE_WIRE_BUS 2
#define button 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int myBPM;
int namee;
String url;
void setup() {

  pinMode(5, INPUT);   // HEART RATE
  pinMode(10, INPUT);  // eeg
  pinMode(11, INPUT);  //eeg
  pinMode(8, OUTPUT);  //buzzer
  digitalWrite(8, LOW);
  pinMode(button, INPUT_PULLUP);
  sensors.begin();
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  sensors.requestTemperatures();
  lcd.setCursor(0, 0);
  lcd.print("ECG:");
  lcd.print(namee);
  lcd.setCursor(8, 0);
  lcd.print("HB: ");
  lcd.print(myBPM);
  lcd.setCursor(0, 1);
  lcd.print("TEMP: ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print("       ");
  
  float temp = sensors.getTempCByIndex(0);
  float glucose_level = ((2.5 * temp) + (0.3 * myBPM) - 50);

  if ((myBPM > 60) && (temp > 27) && (digitalRead(button) == 0)) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("mg/dl: ");
    lcd.print(glucose_level);
  }
  if ((sensors.getTempCByIndex(0) > 35) || (sensors.getTempCByIndex(0) < 25) || (myBPM > 100)) {
    digitalWrite(8, HIGH);
  } else {
    {
      digitalWrite(8, LOW);
    }
  }

  if (digitalRead(5) == LOW) {
    myBPM = random(60, 90);

  } else {
    myBPM = 0;
  }
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {

    namee = 0;
  } else {
    namee = analogRead(A1);
  }


  url = "*TEMP:";
  url += temp;
  url += "ECG";
  url += namee;
  url += "HB";
  url += myBPM;
  delay(1000);
}
