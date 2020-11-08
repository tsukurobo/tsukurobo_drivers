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

#define signal_dot 1000

#define enc_p_rotation 1024
#define wheel_r 0.1



volatile long int counter = 0;

volatile long double v = 0;

<<<<<<< HEAD
const float Kp=49;//1950.4;
const float Ki=0;
const float Kd=0.01;//3.9111;
=======
const float Kp=49;
const float Ki=0;
const float Kd=0.01;
>>>>>>> 0ce3a32715bd8f06eee9079dc95cb0fd2ee591ee
long double targetv=0;


float P=0;
float preP=0;
float I=0;
float D=0;
float duty=0;
int pw = 0;
long double enc = 0;
long double preenc =0;

int I2C_address = 0x11;
Ti2c receiver(I2C_address);



ISR(PCINT1_vect,ISR_NOBLOCK){//エンコーダのＡＢ層の立ち上がりを見てカウントダウンカウントアップする
  if((PINC & (1<<PINC0))^((PINC & (1<<PINC1))>>1))--counter;
  else ++counter;

  }


void setup() {
  PCICR |= (1<<PCIE1);//ピン割り込み設定
  PCMSK1 |=(1<<PCINT8);

  TCCR0B = (TCCR0B & 0b11111000) | 0x03; //pwm 1[kHz]

  Serial.begin (9600);

  pinMode(PWML, OUTPUT);
  pinMode(PWMH, OUTPUT);
  pinMode(SR,OUTPUT); 
  pinMode(dir, OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);

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
    v = (enc - preenc)*wheel_r*2*M_PI/(dt*enc_p_rotation);//速度を計算

    P = targetv - v;

    I += P*dt;

    D = (P - preP)/dt;

    duty = Kp * P + Ki * I + Kd * D;

    preP = P;
    preenc = enc;

    if(duty > max_duty) duty = max_duty;//dutyの上限下限を設定
    if(duty < min_duty) duty = min_duty;

    if(duty>=0)PORTD &= ~_BV(PD7);//digitalWrite(dir,LOW);dutyの値の正負でモータのcw ccwを切り替える
    else PORTD |= _BV(PD7);//digitalWrite(dir,HIGH);

    pw = abs(int(duty));

//モードラにｐｗｍを出力
    analogWrite(PWMH,pw);

  }




void receiveEvent(int howMany)//arduino側から速度指令を受け取った時に呼び出される
{
  receiver.receiveStr();
  targetv = double(atoi(receiver.buf))/signal_dot;//速度指令を格納.signal_dotで割って小数に戻す[m/s]

  P = 0;//速度指令が送られるごとにＰＩＤのパラメータを初期化
  preP = 0;
  D = 0;
  I = 0;


}

void requestEvent(){

  char buf[10] = {};
  int num;
  num = sprintf(buf, "%d$", int(v*signal_dot));//ardunioがencorder関数を呼び出したら速度を返す

  receiver.sendStr(buf);


  }
