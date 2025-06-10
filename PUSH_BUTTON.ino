int button;
void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  digitalWrite(2,HIGH);
  pinMode(3,OUTPUT);
  
}
void loop() {
  // put your main code here, to run repeatedly:
button=digitalRead(2);
  if(button == LOW)
  {
    digitalWrite(3,HIGH);
  }
  else{
digitalWrite(3,LOW);
  }
    

}
