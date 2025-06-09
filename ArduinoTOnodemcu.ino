#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11


void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // float hic = dht.computeHeatIndex(t, h, false);

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));
  URL1 = "HUMIDITY :";
  URL1 += h;
  URL2 = "TEMPERATURE :";
  URL2 += t;
  Serial.println(URL1);
  Serial.println(URL2);
  Serial.println("-----------------------------------------------------------------------");
  delay(1000);
}
