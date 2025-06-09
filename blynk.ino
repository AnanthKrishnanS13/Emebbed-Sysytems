
#define BLYNK_TEMPLATE_ID "TMPL3hQPqiCIi"
#define BLYNK_TEMPLATE_NAME "led"
#define BLYNK_AUTH_TOKEN "qsZG3Vkjn0qn2EA1tSeKKphzozSNlYHb"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Replace with your Wi-Fi credentials
char ssid[] = "ananth";
char pass[] = "11223344";

// LED pin
int ledPin = 2;

void setup()
{
  // Start serial communication
  Serial.begin(115200);

  // Set pin mode
  pinMode(ledPin, OUTPUT);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// This function is called every time the app writes to Virtual Pin V0
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt(); // Get value (0 or 1)
  digitalWrite(ledPin, pinValue); // Turn LED on/off
}

void loop()
{
  Blynk.run();
}
