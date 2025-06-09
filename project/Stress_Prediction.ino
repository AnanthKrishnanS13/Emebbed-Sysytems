// SXCCE IT HEALTH MONITORING

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define ONE_WIRE_BUS 5     // DS18B20 temperature sensor
#define HB_SENSOR_PIN 14   // Heartbeat sensor input
#define ECG_SENSOR_PIN 34  // ECG analog input
#define LEAD_OFF_1 26
#define LEAD_OFF_2 25

const char* ssid = "HEALTH";
const char* password = "12341234";
const char* server = "http://api.thingspeak.com/update";
String apiKey = "CHLIEYLP5Y6MV81J";  // Use your actual ThingSpeak API key

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Global variables
float tempC = 0;
int hb = 0, stableHB = 0;
int ecgValue = 0, stableECG = 0;
unsigned long hbHoldStartTime = 0;
unsigned long ecgHoldStartTime = 0;
bool isHoldingHB = false;
bool isHoldingECG = false;
int displayECG = 0;

unsigned long lastUpdate = 0;
const int updateInterval = 10000; // 10 seconds

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   STROKE");
  lcd.setCursor(0, 1);
  lcd.print("PREDICTION ");
  delay(3000);
  lcd.clear();

  // Initialize sensors
  sensors.begin();
  pinMode(HB_SENSOR_PIN, INPUT);
  pinMode(LEAD_OFF_1, INPUT);
  pinMode(LEAD_OFF_2, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");
}

void loop() {
  static unsigned long lastSensorRead = 0;
  const int sensorInterval = 200;

  // Sensor Read Every 200 ms
  if (millis() - lastSensorRead >= sensorInterval) {
    lastSensorRead = millis();

    // Read temperature
    sensors.requestTemperatures();
    tempC = sensors.getTempCByIndex(0);

    // Heartbeat logic
    if (!isHoldingHB) {
      if (digitalRead(HB_SENSOR_PIN) == LOW) {
        hb = random(55, 85);  // Simulated BPM
        if (hb > 55) {
          stableHB = hb;
          hbHoldStartTime = millis();
          isHoldingHB = true;
        }
      }
    } else if (millis() - hbHoldStartTime >= 5000) {
      isHoldingHB = false;
      
    }

    // ECG read and lead-off detection
    ecgValue = analogRead(ECG_SENSOR_PIN);
    Serial.println(ecgValue);
    bool leadOff = (digitalRead(LEAD_OFF_1) == HIGH) || (digitalRead(LEAD_OFF_2) == HIGH) || (ecgValue > 4000)||(ecgValue <10);

    if (!leadOff) {
      if (!isHoldingECG) {
        stableECG = random(120, 200);  // Simulated ECG
        ecgHoldStartTime = millis();
        isHoldingECG = true;
      }
    }

    if (isHoldingECG && (millis() - ecgHoldStartTime >= 5000)) {
      isHoldingECG = false;
      
    }

    displayECG = stableECG;

    // Display on LCD
    lcd.setCursor(0, 0);
    if (leadOff) {
      lcd.print("Lead Off!     ");
    } else {
      lcd.print("ECG:");
      lcd.print(displayECG);
      lcd.print(" ms   ");
    }

    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(tempC);
    lcd.print("C ");

    lcd.setCursor(9, 1);
    lcd.print(" HB:");
    lcd.print(stableHB);
    lcd.print("  ");

    // Debug info
    Serial.print("Temp: ");
    Serial.print(tempC);
    Serial.print(" C, HB: ");
    Serial.print(stableHB);
    Serial.print(" bpm, ECG: ");
    Serial.println(displayECG);
  }

  // Upload to ThingSpeak every 10 seconds
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String(server) + "?api_key=" + apiKey +
                   "&field1=" + String(tempC) +
                   "&field2=" + String(displayECG) +
                   "&field3=" + String(stableHB);

      Serial.println("Sending data to ThingSpeak...");
      http.begin(url);
      int responseCode = http.GET();
      if (responseCode > 0) {
        Serial.println("Data sent successfully!");
      } else {
        Serial.println("Failed to send data.");
      }
      http.end();
    } else {
      Serial.println("WiFi disconnected.");
    }
  }
}
