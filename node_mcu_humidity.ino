#include <DHT.h>
DHT11 dht(24);
void setup() {
Serial.begin(115200);
}

void loop()
 {
  delay(1000);
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  float tempC = dht.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht.readTemperature();

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF))
   {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }
}
