// defining the pins
const int trigPin =5;
const int echoPin = 6;
// defining variables
long duration;
int distance;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}

  // put your main code here, to run repeatedly:
void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(10);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
delay(100);
}
// ---------------------------base usonic -1
// yellow -- gnd
// orange-- vcc
// brown --trig 9
// blue-- echo
//  -------------------------------------usonic -2
//  purple -- vcc
//  gray -- trig
//  white-- echo
//  green --gnd



