/*
 * ContorolMotor.cpp
 *
 * Created: 2018/01/21 19:11:22
 *  Author: magon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
//20khz
//10khz
//1khz
const int TOP=255;
enum{
	TMR0Full = TOP,//255,
	TMR0INI = 0, //122,
};

void motor_init(void)
{
	//setup
	DDRD |= 0b11110000;
	
	TCCR0A = 0b00100011;  //highspeedPWM, OCR0AのPWMは使用しない
	
	TCCR0B = 0b00000010;  //分周比1/8,Top=OVF=255

  PORTD |= 0b01110000; //SR active
}

/*int pwm_map (int duty){
	return TMR0INI + (int)(TOP/100*duty); //duty=0~100
}*/
	
	
	
void motor_set_speed (int power) {
	
    //OCR0A = pwm_map(abs(power));
    OCR0B = abs(power);
		if (power < 0){
			//PORTD = 0b01110000;	  //PHASE=high
			PORTD &= 0b01111111;
		}
		
		else if(power > 0){
			//PORTD = 0b11100000;   //PHASE=low
			PORTD |= 0b10000000;
		}
				
}
