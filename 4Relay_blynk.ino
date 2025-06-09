#define BLYNK_TEMPLATE_ID "TMPL3UoK5gPa7"
#define BLYNK_TEMPLATE_NAME "Smart home automation"
#define BLYNK_AUTH_TOKEN "vZxjrgIPcYLINfywyfZO64jH1vorGvnr"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 14
DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi credentials
char ssid[] = "Akash";
char pass[] = "akash123";

// Relay GPIOs
#define RELAY1 2
#define RELAY2 4
#define RELAY3 18
#define RELAY4 19

#define IR 13
#define LDR 32

// Variables
float t, h;
bool relay1_blynk = false;
bool relay2_blynk = false;
bool relay3_blynk = false;
bool relay4_blynk = false;

BLYNK_WRITE(V0) { relay1_blynk = param.asInt(); }
BLYNK_WRITE(V1) { relay2_blynk = param.asInt(); }
BLYNK_WRITE(V2) { relay3_blynk = param.asInt(); }
BLYNK_WRITE(V3) { relay4_blynk = param.asInt(); }

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(IR, INPUT);
  pinMode(LDR, INPUT);

  // Relays OFF at startup (HIGH if active LOW)
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  t = dht.readTemperature();
  h = dht.readHumidity();

  // Send temperature and humidity to Blynk app virtual pins
  Blynk.virtualWrite(V4, t);
  Blynk.virtualWrite(V5, h);

  bool relay1_sensor = false;
  bool relay2_sensor = false;
  bool relay3_sensor = false;
  bool relay4_sensor = false;

  // Sensor-based relay logic
  if (t > 32 && digitalRead(IR) == LOW) {
    relay4_sensor = true;
  }

  // LDR logic inverted: ON when no light
  if (digitalRead(LDR) == LOW) {  // Light detected
    relay3_sensor = false;         // Relay OFF
  } else {                         // No light
    relay3_sensor = true;          // Relay ON
  }

  if (digitalRead(IR) == LOW) {
    relay1_sensor = true;
    relay2_sensor = true;
  }

  // Relay control: ON if Blynk OR sensors say ON
  digitalWrite(RELAY1, (relay1_blynk || relay1_sensor) ? LOW : HIGH);
  digitalWrite(RELAY2, (relay2_blynk || relay2_sensor) ? LOW : HIGH);
  digitalWrite(RELAY3, (relay3_blynk || relay3_sensor) ? LOW : HIGH);
  digitalWrite(RELAY4, (relay4_blynk || relay4_sensor) ? LOW : HIGH);

  // Optional debugging
  Serial.print("Temp: "); Serial.print(t); Serial.print(" C, ");
  Serial.print("Humidity: "); Serial.print(h); Serial.println(" %");
  Serial.print("IR: "); Serial.println(digitalRead(IR));
  Serial.print("LDR: "); Serial.println(digitalRead(LDR));  // Use digitalRead for clarity

  delay(500);
}
