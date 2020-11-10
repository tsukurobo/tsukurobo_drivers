#ifndef IseMotorDriver_h
#define IseMotorDriver_h
#include "Arduino.h"

#define STANDARD 0
#define FAST 1

class IseMotorDriver {
  public:
    IseMotorDriver(uint8_t i2caddr);
    static void begin(uint8_t mode);
    void setSpeed(int power);
    long encoder();
  private:
    uint8_t addr;
    //Ti2c ti2c;
};
#endif
