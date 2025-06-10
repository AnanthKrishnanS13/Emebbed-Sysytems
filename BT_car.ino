const int m1_pin1 =3;
const int m1_pin2 =4;
const int m2_pin1 =6;
const int m2_pin2 = 7;
String dir;
String matt;


void forward()
{
  //motor1
  digitalWrite(m1_pin1,HIGH);
  digitalWrite(m1_pin2,LOW);
  //motor2
  digitalWrite(m2_pin1,HIGH);
  digitalWrite(m2_pin2,LOW);

}
void backward()
{
  
  digitalWrite(m1_pin1,LOW);
  digitalWrite(m1_pin2,HIGH);
  digitalWrite(m2_pin1,LOW);
  digitalWrite(m2_pin2,HIGH);

}
void left()
{
  //motor1
  digitalWrite(m1_pin1,HIGH);
  digitalWrite(m1_pin2,LOW);
  //motor2
  digitalWrite(m2_pin1,LOW);
  digitalWrite(m2_pin2,HIGH);

}
void right(){
  //motor1
  digitalWrite(m1_pin1,LOW);
  digitalWrite(m1_pin2,HIGH);
  //motor2
  digitalWrite(m2_pin1,HIGH);
  digitalWrite(m2_pin2,LOW);

}
void stop()
{
  digitalWrite(m1_pin1,LOW);
  digitalWrite(m1_pin2,LOW);
  digitalWrite(m2_pin1,LOW);
  digitalWrite(m2_pin2,LOW);

}
void setup() {
  Serial.begin(9600);
  pinMode(m1_pin1,OUTPUT);
  pinMode(m1_pin2,OUTPUT);
  pinMode(m2_pin1,OUTPUT);
  pinMode(m2_pin2,OUTPUT);
  
}

void loop() 
{
 // backward();
  while (Serial.available() > 0) 
  {
    
    //char dir = (char)Serial.read();
    dir=Serial.readString();
    Serial.println(dir);

 if(dir == "forward" )
 {
  forward();
 }
  if(dir == "backward")
 {
  backward();
 }
 if(dir == "right" )
 {
  right();
 }
 if(dir == "left" )
 {
  left();
 }
 if(dir == "stop")
 {
  stop();
 }

  }
}
