#ifndef IseMotorDriver_h
#define IseMotorDriver_h
#include "Arduino.h"

#define STANDARD 0
#define FAST 1

//TOP=255
#define PWM_64KHZ 0x01
#define PWM_8KHZ 0x02
#define PWM_1KHZ 0x03

#define SM_BRAKE_LOW_SIDE 0x00
#define SM_BRAKE_HIGH_SIDE 0x01
#define SM_COAST 0x02
#define LAP 0x03 //伊勢モードラでは使用不可

class IseMotorDriver {
  public:
    IseMotorDriver(uint8_t i2caddr);
    void setMode(uint8_t freq, uint8_t mode);
    static void begin(uint8_t mode);
    void setSpeed(int power);
    long encoder();
  private:
    uint8_t addr;
    //Ti2c ti2c;
};
#endif
