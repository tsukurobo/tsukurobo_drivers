
 /*version 3_for_driving_system*/
#define F_CPU 20000000UL

#include <avr/io.h>
#include "I2CSlave.h"
#include "motordriver.h"
#include <avr/interrupt.h>

#define I2C_ADDR 0x61
#define PWM_64KHZ 0x01
#define MSB 0x80
#define BIT2 0x40
#define BIT1 0x20
#define READ_BIT0_TO_4 0x1F
#define READ_BIT0_TO_2 0x07
#define READ_BIT0_TO_6 0x7F

volatile long count = 0;
volatile int power = 0;

void i2c_received_cb(uint8_t data) {//送られてきた2バイトのデータを結合
  volatile static uint8_t sign_flag = 0, seq_flag = 0;
  volatile static unsigned int power_raw=0;
  static int power;
  
  if((data&MSB) == 0){//前半部分のデータが来た場合
    if((data&BIT2) == 0){ //速度指令の場合
      sign_flag = ((data&BIT1)==0) ? 0 : 1 ; //符号ビットを確認し、sign_flagの値を設定
      power_raw = (unsigned int)(data&READ_BIT0_TO_4)<<7; //速度指令(前半)を取り出し、7ビット左シフトさせてpower_rawに代入
      seq_flag = 1;
    }else{ //MD設定用のデータの場合
      motor_init(data&READ_BIT0_TO_2, (data>>3)&READ_BIT0_TO_2); //モーターの回転設定を行う
      seq_flag = 1;
      
    }
  }else{ //後半部分のデータが来た場合
    if(seq_flag == 1){
      power_raw |= (unsigned int)(data&READ_BIT0_TO_6); //速度指令(後半)を取り出し、power_rawとのORを取る
      power = (sign_flag==0) ? (int)power_raw : -(int)power_raw; //sign_flagに応じて-1倍する
      motor_set_speed(power); //モーターを回転させる
      //Serial.println(power);
      seq_flag = 0;
    }
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
  
	motor_init(SM_BRAKE_LOW_SIDE, PWM_64KHZ); //デフォルトの設定
	motor_set_speed(power);
	
	DDRC = 0x00;
	PORTC = 0x00;
	
	// LED of addresses 0x10~ 0x1F 0~F -> 0~15
	PORTD |= (0b00001111 & I2C_ADDR);

	//sei();
  //intial settings
	//I2C_init(I2C_ADDR); //I2Cの割り込みオン(使うべきでないぞ)
  I2C_init_sync(I2C_ADDR);
  I2C_setCallbacks(i2c_received_cb, i2c_requested_cb);
  //Serial.begin(57600);

}


void loop() {
  I2C_main();
}
