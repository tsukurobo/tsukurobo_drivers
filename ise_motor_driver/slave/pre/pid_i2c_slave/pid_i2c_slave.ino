#include <TimerOne.h>
#include <Wire.h>

#include "ti2c_slave.h"


#define PWMH 5
#define PWML 6
#define dir 7
#define SR 4

#define max_duty 255
#define min_duty -255
#define dt 0.003

#define enc_p_rotation 4048


volatile long int temp, counter = 0;
const float Kp=0.25;
const float Ki=0.01;
const float Kd=0.00059;
float targetenc=0;


float P=0;
float preP=0;

float I=0;
float D=0;
float duty=0;
int pw = 0;

float enc = 0;
float preenc;


int I2C_address = 0x14;
Ti2c receiver(I2C_address);

ISR(PCINT1_vect,ISR_NOBLOCK){//エンコーダのＡＢ層の立ち上がりを見てカウントダウンカウントアップする
  if((PINC & (1<<PINC0))^((PINC & (1<<PINC1))>>1))--counter;
  else ++counter;

  }


void setup() {
  PCICR |= (1<<PCIE1);//ピン割り込み設定
  PCMSK1 |=(1<<PCINT8);
  TCCR0B = (TCCR0B & 0b11111000) | 0x03; //pwm 1 [kHz]

  Serial.begin (9600);

  pinMode(PWML, OUTPUT);
  pinMode(PWMH, OUTPUT);
  pinMode(SR,OUTPUT);
  pinMode(dir, OUTPUT);


  digitalWrite(SR,HIGH);//モードラの設定
  digitalWrite(PWML,HIGH);

  Timer1.initialize(3000);//PIDの制御周期用のタイマー
  Timer1.attachInterrupt(PID);

  Wire.begin(I2C_address);   // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);

}




void loop() {

}


void PID(){
    enc = counter;

    P = targetenc - enc;

    I += P*dt;

    D = (P - preP)/dt;

    duty = Kp * P + Ki * I + Kd * D;

    preP = P;
    if(duty > max_duty) duty = max_duty;//dutyの上限下限を設定
    if(duty < min_duty) duty = min_duty;

    if(duty>=0)PORTD &= ~_BV(PD7);//digitalWrite(dir,LOW);dutyの値の正負でモータのcw ccwを切り替える
    else PORTD |= _BV(PD7);//digitalWrite(dir,HIGH);


    pw = abs(int(duty));
    //モードラにｐｗｍを出力
    analogWrite(PWMH,pw);
  }




void receiveEvent(int howMany)//arduino側から角度指令を受け取った時に呼び出される
{
  receiver.receiveStr();
  targetenc = double(atoi(receiver.buf));//角度指令格納

  P = 0;//速度指令が送られるごとにＰＩＤのパラメータを初期化
  preP = 0;
  I = 0;
  D = 0;
}

void requestEvent(){

  char buf[10] = {};
  int num;
  num = sprintf(buf, "%d$", counter);//ardunioがencorder関数を呼び出したらエンコーダの値を返す


  receiver.sendStr(buf);



  }
