#include <ezButton.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int m1 = 4;
int m2 = 5;
int en = 3;
#define R 6371.0
int key = 0;
#define KEY_NUM 4
String seats = "";
String available_seats = "";

#define PIN_KEY_1 9
#define PIN_KEY_2 8
#define PIN_KEY_3 11
#define PIN_KEY_4 10
int buzzerPin = 12;
static unsigned long seatInputTime = 0;
static bool seatJustEntered = false;  // NEW flag to ignore initial key

// Default locations
double lat1 = 8.207685;
double lon1 = 77.399091;

double lat2 = 8.515227;
double lon2 = 76.898509;

double lat3 = 8.078542;
double lon3 = 77.550945;

double lat4 = 9.150167;
double lon4 = 79.449387;
String TO = "";

int selectingDestination = 0;
int selectedDestination = 0;
unsigned long previousMillis = 0;
const long interval = 500;  // 500ms beep interval
bool buzzerState = false;
float ETA = 0;
int countflag = 0;
#define SPEED_SENSOR_PIN A0

unsigned long lastKeyTime = 0;

ezButton keypad_1x4[] = {
  ezButton(PIN_KEY_1),
  ezButton(PIN_KEY_2),
  ezButton(PIN_KEY_3),
  ezButton(PIN_KEY_4)
};

// --- GPS Setup ---
SoftwareSerial gpsSerial(7, 6);  // RX, TX (adjust pins as needed)
TinyGPSPlus gps;

double currentLat = lat1;  // fallback default lat
double currentLon = lon1;  // fallback default lon
bool gpsFix = false;

double toRadians(double degree) {
  return degree * (M_PI / 180.0);
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
  double dlat = toRadians(lat2 - lat1);
  double dlon = toRadians(lon2 - lon1);

  lat1 = toRadians(lat1);
  lat2 = toRadians(lat2);

  double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = R * c;

  return distance;
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);  // GPS baud rate

  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(en, OUTPUT);

  for (byte i = 0; i < KEY_NUM; i++) {
    keypad_1x4[i].setDebounceTime(10);
  }
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("     BUS    ");
  lcd.setCursor(0, 1);
  lcd.print("   MONITORING   ");
  delay(3000);
  lcd.clear();
}

void loop() {
  // Read GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    currentLat = gps.location.lat();
    currentLon = gps.location.lng();
    
  } else {
    currentLat = lat1;  // fallback default lat
    currentLon = lon1;  // fallback default lon

  }

  int key = getKeyPressed();

  // Simple debounce: ignore repeated keypresses within 500ms
  if (millis() - lastKeyTime < 500) {
    key = 0;
  }

  if (selectingDestination == 0 && key == 3 && countflag == 0) {
    selectingDestination = 1;
    lastKeyTime = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select Dest: 1-3");
    TO="SELECT DESTINATION";
    return;
  }

  if (selectingDestination == 1 && key >= 1 && key <= 3) {
    selectedDestination = key;
    selectingDestination = 0;
    lastKeyTime = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dest Selected:");
    lcd.setCursor(0, 1);
    //lcd.print("Location ");
    if (selectedDestination == 1) {
      lcd.print("LULU MALL ");
      TO="LULU MALL";
    }
    if (selectedDestination == 2) {
      lcd.print("KANYAKUMARI BEACH ");
      TO="KANYAKUMARI BEACH";
    }
    if (selectedDestination == 3) {
      lcd.print("RAMESHWARAM   ");
      TO="RAMESHWARAM";
    }

    delay(2000);
    lcd.clear();
  }

  if ((key == 4) && (countflag == 0)) {
    countflag = 1;
    seats = "";
    seatJustEntered = true;  // set flag to ignore initial 4
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Seats:");
    delay(200);  // short debounce delay
  }

  if (countflag == 1) {
    if (seatJustEntered && key == 4) {
      key = 0;  // ignore this 4
    } else {
      seatJustEntered = false;  // reset flag after ignoring first press
    }

    if (key >= 1 && key <= 3 && seats.length() < 2) {
      seats += String(key);
      lcd.setCursor(0, 1);
      lcd.print(seats);
      delay(500);
      seatInputTime = millis();  // reset timeout
    }

    // Finalize after timeout (2 seconds after last digit)
    if (seats.length() > 0 && (millis() - seatInputTime > 2000)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Total Seats:");
      lcd.setCursor(0, 1);
      lcd.print(seats);
      available_seats = seats;
      delay(3000);
      seats = "";
      countflag = 0;
      seatInputTime = 0;
      seatJustEntered = false;
      lcd.clear();
    }
  }

  // Only show speed and ETA if destination is selected and seat input is done
  if (countflag == 0 && selectingDestination == 0 && selectedDestination > 0) {
    double distance = 0;

    if (selectedDestination == 1) {
      distance = haversine(currentLat, currentLon, lat2, lon2);
    } else if (selectedDestination == 2) {
      distance = haversine(currentLat, currentLon, lat3, lon3);
    } else if (selectedDestination == 3) {
      distance = haversine(currentLat, currentLon, lat4, lon4);
    }

    int sensorValue = analogRead(SPEED_SENSOR_PIN);
    int speedKmph = map(sensorValue, 0, 1023, 0, 100);

    int b = map(sensorValue, 0, 1023, 42, 255);
    analogWrite(en, b);
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);

    int percent = map(b, 42, 255, 0, 100);
    lcd.setCursor(0, 0);
    lcd.print("SPEED: ");
    lcd.print(percent);
    lcd.print(" km/h  ");

    ETA = (speedKmph > 0) ? (distance / speedKmph) * 60 : 0;

    lcd.setCursor(0, 1);
    lcd.print("ETA: ");
    lcd.print(ETA, 2);
    lcd.print(" mins      ");
    if (speedKmph > 80) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        buzzerState = !buzzerState;  // toggle buzzer
        digitalWrite(buzzerPin, buzzerState ? HIGH : LOW);
      }
    } else {
      digitalWrite(buzzerPin, LOW);
      buzzerState = false;
    }
    
      Serial.print("*");
    Serial.print(currentLat, 6);
    Serial.print(",");
    Serial.print(currentLon, 6);
    Serial.print(",");
    Serial.print(ETA);
    Serial.print(",");
    Serial.print(available_seats);
     Serial.print(",");
    Serial.print(TO);
    Serial.println("#");
  }

  // Display default message if no destination is selected
  if (selectingDestination == 0 && selectedDestination == 0 && countflag == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Select Destination ");
    lcd.setCursor(0, 1);
    lcd.print("Press 3 to Start  ");
  }
}

int getKeyPressed() {
  for (byte i = 0; i < KEY_NUM; i++) {
    keypad_1x4[i].loop();
  }

  for (byte i = 0; i < KEY_NUM; i++) {
    if (keypad_1x4[i].isReleased()) {
      return (i + 1);
    }
  }

  return 0;
}
