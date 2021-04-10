/*
 * main.c
 *
 *  Created on: Feb 23, 2019
 *      Author: root
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "usi_i2c_slave.h"

extern char* USI_Slave_register_buffer[];

volatile unsigned int PulseCount0 = 0;
volatile unsigned int PulseCount1 = 0;
volatile unsigned int PulseCount2 = 0;

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) // disable ADC (before power-off)
#define adc_enable()  (ADCSRA |=  (1<<ADEN)) // re-enable ADC


void setup() {

	DDRB  = 0b00000000; // all inputs
	PORTB = 0b00011010; // internal pull-ups on

	GIMSK = 0b00100000; // PCIE
	PCMSK = 0b00011010; // PCINT4 | PCINT3 | PCINT1

//	GIMSK |= (1 << PCIE);
//	PCMSK |= (1 << PCINT4 ) || (1 << PCINT3) || (1 << PCINT1);



	sei();
	USI_I2C_Init(0x42);
	USI_Slave_register_buffer[0] = (unsigned char*)(&PulseCount0);
	USI_Slave_register_buffer[1] = (unsigned char*)(&PulseCount0)+1;
	USI_Slave_register_buffer[2] = (unsigned char*)(&PulseCount1);
	USI_Slave_register_buffer[3] = (unsigned char*)(&PulseCount1)+1;
	USI_Slave_register_buffer[4] = (unsigned char*)(&PulseCount2);
	USI_Slave_register_buffer[5] = (unsigned char*)(&PulseCount2)+1;
}



ISR(PCINT0_vect) {

	short int PinState;

	PinState = PINB;

	if ((PinState & 0b00000010) == 0) {
		PulseCount0 += 1;
	} else 	if ((PinState & 0b00001000) == 0) {
		PulseCount1 += 1;
	} else 	if ((PinState & 0b00010000) == 0) {
		PulseCount2 += 1;
	};

}


int main(void) {

	setup();

	while (1) {

	    cli();
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		adc_disable();
		sleep_enable();
		sleep_bod_disable();
		sei();
		sleep_cpu();
		sleep_disable();
		
	}
	return(0);

}



