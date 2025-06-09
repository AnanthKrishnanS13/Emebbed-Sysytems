#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
char received;

// Define pins
const int voltageSensor1 = A0;
const int voltageSensor2 = A1;
const int relay1 = 8;
const int relay2 = 9;
// Voltage divider resistors
float R1 = 30000.0;
float R2 = 7500.0;
float ref_voltage = 5.0;
int motor_pin1 = 4;
int motor_pin2 = 5;
int motor_pin3 = 6;
int motor_pin4 = 7;

// Timing variables
unsigned long lastBatteryUpdate = 0;
unsigned long batteryMsgInterval = 3000; // 3 seconds
int batteryState = 0; // 0: idle, 1: B1 ON, 2: B2 Charging, 3: B2 ON, 4: B1 Charging

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("HYBRID BATTERY ");
  lcd.setCursor(0, 1);
  lcd.print("   MANAGEMENT  ");
  delay(3000);
  lcd.clear();

  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pin3, OUTPUT);
  pinMode(motor_pin4, OUTPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

}

void loop() {
  // Read voltages
  int adc_value1 = analogRead(voltageSensor1);
  int adc_value2 = analogRead(voltageSensor2);
  float voltage1 = (adc_value1 * ref_voltage / 1023.0) * ((R1 + R2) / R2);
  float voltage2 = (adc_value2 * ref_voltage / 1023.0) * ((R1 + R2) / R2);

  // Print to Serial
  // Serial.print("Voltage 1: ");
  // Serial.print(voltage1);
  // Serial.print(" V | Voltage 2: ");
  // Serial.print(voltage2);
  // Serial.println(" V");

  // Update LCD - Line 1
  lcd.setCursor(0, 0);
  lcd.print("V1:");
  lcd.print(voltage1, 2);
  lcd.print("V V2:");
  lcd.print(voltage2, 2);

  // Battery management logic
  unsigned long currentMillis = millis();

  if (voltage1 > voltage2 + 0.1) {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);

    if (currentMillis - lastBatteryUpdate > batteryMsgInterval) {
      lastBatteryUpdate = currentMillis;
      batteryState = (batteryState == 1) ? 2 : 1;
    }

    lcd.setCursor(0, 1);
    if (batteryState == 1) {
      lcd.print("BATTERY 1 ON     ");
    } else {
      lcd.print("BATTERY 2 Charging");
    }

  } else if (voltage2 > voltage1 + 0.1) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);

    if (currentMillis - lastBatteryUpdate > batteryMsgInterval) {
      lastBatteryUpdate = currentMillis;
      batteryState = (batteryState == 3) ? 4 : 3;
    }

    lcd.setCursor(0, 1);
    if (batteryState == 3) {
      lcd.print("BATTERY 2 ON     ");
    } else {
     lcd.print("BATTERY 1 Charging");
    }

  } 
  
  else {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    lcd.setCursor(0, 1);
    lcd.print("BATTERY 1 ON     ");
  }

  // Handle Bluetooth commands
  if (Serial.available()) {
    received = Serial.read();
    Serial.print("Received: ");
    Serial.println(received);

    switch (received) {
      case 'F': Forward(); Serial.println("FORWARD"); break;
      case 'B': Backward(); Serial.println("BACKWARD"); break;
      case 'L': Left(); Serial.println("LEFT"); break;
      case 'R': Right(); Serial.println("RIGHT"); break;
      case 'S': Stop(); Serial.println("STOP"); break;
    }
  }

  delay(100); // Short delay for stability
}

// Motor control functions
void Forward() {
  digitalWrite(motor_pin1, HIGH);
  digitalWrite(motor_pin2, LOW);
  digitalWrite(motor_pin3, HIGH);
  digitalWrite(motor_pin4, LOW);
}
void Backward() {
  digitalWrite(motor_pin1, LOW);
  digitalWrite(motor_pin2, HIGH);
  digitalWrite(motor_pin3, LOW);
  digitalWrite(motor_pin4, HIGH);
}
void Right() {
  digitalWrite(motor_pin1, HIGH);
  digitalWrite(motor_pin2, LOW);
  digitalWrite(motor_pin3, LOW);
  digitalWrite(motor_pin4, HIGH);
}
void Left() {
  digitalWrite(motor_pin1, LOW);
  digitalWrite(motor_pin2, HIGH);
  digitalWrite(motor_pin3, HIGH);
  digitalWrite(motor_pin4, LOW);
}
void Stop() {
  digitalWrite(motor_pin1, LOW);
  digitalWrite(motor_pin2, LOW);
  digitalWrite(motor_pin3, LOW);
  digitalWrite(motor_pin4, LOW);
}
