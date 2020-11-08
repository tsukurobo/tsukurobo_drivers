/*
 * ise_motor_driver.cpp
 * original
 * Created : 2018/03/09 19:51:04
 * Author : Yoshihara(kari)
 */ 

 /*version 2*/
#define F_CPU 20000000UL

#include <avr/io.h>
#include "I2CSlave.h"
#include "motordriver.h"
#include <avr/interrupt.h>

#define I2C_ADDR 0x14


volatile long count = 0;
volatile int power = 0;

void i2c_received_cb(uint8_t data) {//送られてきた2バイトのデータを結合
  volatile static uint8_t flag = 0;
  volatile static unsigned int power_raw=0;
  
  if(flag == 0){
    power_raw = (unsigned int)data<<8;
    flag++;
  }else if(flag == 1){
    power_raw |= (unsigned int)data;
    power = (int)power_raw;
    flag = 0;
    motor_set_speed(power);
  }
}

void i2c_requested_cb() {
	//long a = -1000;
  volatile static unsigned long x;
  volatile static uint8_t part=0;
  if(part==0) x=count;
  uint8_t y = (x<<part*8)>>24;
  I2C_transmitByte(y); //32ビットを8ビットずつ区切って上位ビットから送る
  part++;
  if(part>=4){
    part=0;
    //count -= 10;
  }
}

ISR(PCINT1_vect, ISR_NOBLOCK){//encorder
	if((PINC & (1 << PINC0)) ^ ((PINC & (1 << PINC1))>> 1))--count;
	else ++count;	
}


void setup (){
	
	//ピン変化割り込み許可（PCINT8~15）
	PCICR |= (1<<PCIE1);
	////ピン変化割り込み許可（PCINT8）
	PCMSK1 |= (1<<PCINT8);
	motor_init();
	motor_set_speed(power);
	
	DDRC = 0x00;
	PORTC = 0x00;
	
	// LED of addresses 0x10~ 0x1F 0~F -> 0~15
	PORTD |= (0b00001111 & I2C_ADDR);

	//sei();
  //intial settings
	I2C_init_sync(I2C_ADDR);
  I2C_setCallbacks(i2c_received_cb, i2c_requested_cb);
  //Serial.begin(57600);
}


	
    /* Replace with your application code */
void loop() {
	I2C_main();
  //Serial.println(power);
}
