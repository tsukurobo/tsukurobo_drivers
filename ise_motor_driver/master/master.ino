#include <Wire.h>
#include "ise_motor_driver.h"

const uint8_t addr = 0x21; //AVRのアドレスに合わせて変更
int pw = 30; //モータに送るパワー(duty比) [-255~255]

IseMotorDriver mot(addr);

void setup(){
  Wire.begin();
  Serial.begin(9600);
  Serial.println("start");
  mot.setSpeed(0);
}

void loop(){
  long enc = 0; //エンコーダの値
  
  mot.setSpeed(pw); //パワーを送信する関数
  enc = mot.encorder(); //エンコーダ値を得る関数

  Serial.println(enc);
  delay(10);
}
