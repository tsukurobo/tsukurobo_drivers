#include "ise_motor_driver_v3.h"

//伊勢モードラ用新コードのテストプログラム
long e;
IseMotorDriver md(0x61);
void setup() {
  IseMotorDriver::begin();
  if(!md==false) md << IseMotorDriver::createSettingData(PWM_64KHZ,SM_BRAKE_LOW_SIDE);
  Serial.begin(57600);
}

void loop() {
  static long time_pre = millis(), time_pre2 = millis();
  static uint8_t flag=0;
  long time_now, time_now2;
  
  time_now = millis();
  time_now2 = time_now;
  if(time_now - time_pre>=50){
    //long time_now3 = micros();
    if (!md == false){
      //int dura = micros() - time_now3;
      md >> e;
      //Serial.println(dura);
      Serial.println(e);
      //Serial.print(" : ");
    }else{
      Serial.println("md not connected");
    }
    time_pre = time_now;
  }
  if(time_now2 - time_pre2>=1000){
    md << ((flag==0) ? -200 : 0);
    flag = !flag;
    time_pre2 = time_now2;
  }

}
