#define BLYNK_TEMPLATE_ID "TMPL3nBJwZ-_L"
#define BLYNK_TEMPLATE_NAME "BUS MONITORING"
#define BLYNK_AUTH_TOKEN "KXWeIzBs6EXaUnjHv0ITRaYV5ypigSas"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

char ssid[] = "CAMERA";
char pass[] = "12341234";

BlynkTimer timer;
String FROM = "PONJESLY COLLLEGE OF ENGINEERING,NAGERCOIL";
String receivedData = "";
bool messageStarted = false;
float lat;
float lon;
float eta;
int seats;
String TO = "";
void setup() {
  Serial.begin(115200);  // ESP8266 debug
  Serial.println("start..");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send data to Blynk every 1 second
  timer.setInterval(3000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '*') {
      receivedData = "";
      messageStarted = true;
    } else if (c == '#') {
      messageStarted = false;
      processData(receivedData);
    } else if (messageStarted) {
      receivedData += c;
    }
  }
}

void sendData() {
  Blynk.virtualWrite(V0, String(lat, 6));
  Blynk.virtualWrite(V1, String(lon, 6));
  Blynk.virtualWrite(V2, eta);
  Blynk.virtualWrite(V3, seats);
  Blynk.virtualWrite(V4, FROM);
  Blynk.virtualWrite(V5, TO);
}


void processData(String data) {
  Serial.println("Raw Data: " + data);

  lat = data.substring(0, data.indexOf(',')).toFloat();
  data = data.substring(data.indexOf(',') + 1);

  lon = data.substring(0, data.indexOf(',')).toFloat();
  data = data.substring(data.indexOf(',') + 1);

  eta = data.substring(0, data.indexOf(',')).toFloat();
  data = data.substring(data.indexOf(',') + 1);

  seats = data.substring(0, data.indexOf(',')).toInt();
  data = data.substring(data.indexOf(',') + 1);

  TO = data;  // remaining string is destination name


  Serial.print("Latitude: ");
  Serial.println(lat, 6);
  Serial.print("Longitude: ");
  Serial.println(lon, 6);
  Serial.print("ETA: ");
  Serial.print(eta);
  Serial.println(" min");
  Serial.print("Seats: ");
  Serial.println(seats);
}
