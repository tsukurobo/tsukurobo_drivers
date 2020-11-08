#define PIN_SOLENOID 5 //ピン番号

void setup(){
  pinMode(PIN_SOLENOID, OUTPUT);
  
  digitalWrite(PIN_SOLENOID, LOW);
}

void loop(){
  digitalWrite(PIN_SOLENOID,HIGH);
  delay(1000);
  digitalWrite(PIN_SOLENOID,LOW);
  delay(1000);
}
