//sm方式（ショートブレーキあり）
//lap方式（ショートブレーキ無し）

//両方向sm方式
//PWMH :PWM
//PWML :HIGH
//PHASE:HIGH
//SR   :HIGH

//両方向rap方式
//PWMH :HIGH
//PWML :HIGH
//PHASE:PWM
//SR   :HIGH

#include <TimerOne.h>
#include <Wire.h>
#include "ti2c_slave.h"

#define encA 2
#define encB 3
#define PWMH 5
#define PWML 6
#define dir 7
#define SR 4

volatile long int counter = 0;
long double enc = 0;

float duty=0;
int pw = 0;

int I2C_address = 0x21;
Ti2c receiver(I2C_address);

ISR(PCINT1_vect,ISR_NOBLOCK){
  if((PINC & (1<<PINC0))^((PINC & (1<<PINC1))>>1))--counter;
  else ++counter;
}


void setup() {
  PCICR |= (1<<PCIE1);
  PCMSK1 |=(1<<PCINT8);
  
  Serial.begin (9600);
  
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
  pinMode(PWML, OUTPUT);
  pinMode(PWMH, OUTPUT);
  pinMode(SR,OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(A1,INPUT);

  Wire.begin(I2C_address);   // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  _SFR_BYTE(TCCR2A) |= _BV(COM2A1);
  // fast PWM mode
  // 動作クロックは分周なしの8MHz
  // PWMキャリア波の周波数は8MHz/256=31.25kHz

  digitalWrite(SR,HIGH);/////////////////////////////////////////////////////////////
}



 
void loop() {
  enc = counter;

  if(pw>0)digitalWrite(dir,LOW);//CW・CCWの切り替え
  else digitalWrite(dir,HIGH);
  
  duty = abs(int(pw));
  
  digitalWrite(PWML,HIGH);//analogWrite(PWML,pw);//SM方式////////////////////////////
  analogWrite(PWMH,duty);
}

void receiveEvent(int howMany){
  receiver.receiveStr();
  pw = double(atoi(receiver.buf));
}

void requestEvent(){
  char buf[10] = {};
  int num;
  num = sprintf(buf, "%d$", counter);
;

  receiver.sendStr(buf);
  //Serial.println(buf);   
}
