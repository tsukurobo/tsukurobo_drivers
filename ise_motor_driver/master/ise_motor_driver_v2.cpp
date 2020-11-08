#include "Arduino.h"
#include "ise_motor_driver_v2.h"
#include <Wire.h>

// IseMotorDriver::IseMotorDriver(uint8_t i2caddr) {}
// void IseMotorDriver::setSpeed(int power){}
// long IseMotorDriver::encoder(){}

IseMotorDriver::IseMotorDriver(uint8_t i2caddr){
  // @param uint8_t i2caddr: i2c addr of motor driver
  //initializer
  this->addr = i2caddr;
}

void IseMotorDriver::begin(uint8_t mode){
  Wire.begin();
  if(mode == STANDARD) Wire.setClock(100000); //100kbps
  if(mode == FAST) Wire.setClock(400000); //400kbps
}

// 実行には約160μs(@400kbps)かかる
void IseMotorDriver::setSpeed(int power){
  //@param pw(max): -1024~1024
  //16ビットを8ビットずつ分割して送信
  unsigned int data = (unsigned int)power; //符号なしに変換
  
  Wire.beginTransmission(this->addr); //送信開始
  Wire.write(data>>8); //上位8ビット送信
  Wire.write(((data>>8)<<8)^data); //下位8ビット送信
  Wire.endTransmission(); //送信終了
}

// 実行には約280μsかかる(@400kbps)
long IseMotorDriver::encoder(){
  //long count = 0;
  unsigned long buf=0;
  //uint8_t buf2;
  /*char buf[50];
  this -> ti2c.receiveStr(buf);
  count = atol(buf);
  return count;*/
  //for(int i=0; i<4; i++){ //32ビットの値を8ビットずつ分割して取得
    Wire.requestFrom(this->addr, 4); //4バイト要求
    while(Wire.available() < 4 );//データが来るまで待つ
    //buf2 = Wire.read();
    for(int i=0; i<4; i++){
    buf |= (unsigned long)Wire.read()<<(8*(3-i));//データをバッファに格納
    //Serial.print(dura); Serial.print(" ");
    }
  //Serial.println("");
  //Serial.println(buf, HEX);
  //count = (long)buf;
  return (long)buf;
}
