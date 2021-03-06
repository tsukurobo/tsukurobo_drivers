#include "Arduino.h"
#include "ise_motor_driver.h"
#include "ti2c.h"

// IseMotorDriver::IseMotorDriver(uint8_t i2caddr) {}
// void IseMotorDriver::setSpeed(int power){}
// long IseMotorDriver::encoder(){}

IseMotorDriver::IseMotorDriver(uint8_t i2caddr) :
ti2c(i2caddr){
  // @param uint8_t i2caddr: i2c addr of motor driver
  //initializer
  this->addr = i2caddr;
}

// 実行には約600μsかかる
void IseMotorDriver::setSpeed(int power){
  //@param pw: -100~100
  char buf[10];
  sprintf(buf, "%d", power);
  this->ti2c.sendStr(buf); 
}

// 実行には約500μsかかる
long IseMotorDriver::encoder(){
  long count = 0;
  char buf[50];
  this -> ti2c.receiveStr(buf);
  count = atol(buf);
  return count;
}
