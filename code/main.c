/*
 * main.c
 *
 *  Created on: Apr 09, 2021
 *      Author: Pawel Tanski
 */

 // ATMEL ATTINY85
 //
 //                   +-\/-+
 //      reset  PB5  1|    |8  VCC
 //             PB3  2|    |7  PB2  SCL
 //             PB4  3|    |6  PB1
 //             GND  4|    |5  PB0  SDA
 //                   +----+
 //

// PORTB &= ~(1 << PD0); // PD0 goes low
// PORTB |= (1 << PD0); // PD0 goes high

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "usi_i2c_slave.h"

extern char* USI_Slave_register_buffer[];

#define WDG_STATUS_IDLE      0x00
#define WDG_STATUS_ACTIVE    0x01
#define WDG_STATUS_DISABLED  0x02
#define WDG_STATUS_REBOOTING 0x03
#define WDG_STATUS_POWEROFF  0x08

#define WDG_CMD_NOP          0x00
#define WDG_CMD_DISABLE      0x01
#define WDG_CMD_ENABLE       0x02
#define WDG_CMD_FEED         0x03
#define WDG_CMD_REBOOT       0x04
#define WDG_CMD_POWEROFF     0x05
#define WDG_CMD_LINE1_OFF    0x06
#define WDG_CMD_LINE1_ON     0x07
#define WDG_CMD_LINE2_OFF    0x08
#define WDG_CMD_LINE2_ON     0x09
#define WDG_CMD_STORE		     0x0A


// timer0 - główny zegar watchdoga
// timer1 - zegar mierzący jak długo zasilanie ma być wyłączone
// timer2 - zegar mierzący po jakim okresie włączyć zasilanie
// timer3 - zegar mierzący czas zwłoki po jakim WDG się sam włączy

#define WDG_DEFAULT_TIMER0   30 * 10
#define WDG_DEFAULT_TIMER1   10 * 10
#define WDG_DEFAULT_TIMER2   (unsigned int) 3600 * 10
#define WDG_DEFAULT_TIMER3   120 * 10


volatile short int WDG_REG_COMMAND;
volatile short int WDG_REG_CONFIG;
volatile short int WDG_REG_STATUS;
volatile short int WDG_REG_RESERVED;
volatile unsigned int WDG_REG_TIMER0;
volatile unsigned int WDG_REG_TIMER1;
volatile unsigned int WDG_REG_TIMER2;

unsigned int timer0, timer1, timer2, timer3;

void setup() {


//  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//  | ... | ... | PB5 | PB4 | PB3 | PB2 | PB1 | PB0 |
//
// Inputs:
// PB0 - SDA
// PB2 - SCL
//
// Outputs:
// PB3 - 5.0 V
// PB4 - 3.3 V

	DDRB  = 0b00011000; //
	PORTB = 0b00000101; // internal pull-ups on

	cli();
	USI_I2C_Init(0x33);
	USI_Slave_register_buffer[0] = (char*)(&WDG_REG_COMMAND);
	USI_Slave_register_buffer[1] = (char*)(&WDG_REG_CONFIG);
	USI_Slave_register_buffer[2] = (char*)(&WDG_REG_STATUS);
	USI_Slave_register_buffer[3] = (char*)(&WDG_REG_RESERVED);
    USI_Slave_register_buffer[4] = (char*)(&WDG_REG_TIMER0);
	USI_Slave_register_buffer[5] = (char*)(&WDG_REG_TIMER0)+1;
    USI_Slave_register_buffer[6] = (char*)(&WDG_REG_TIMER1);
	USI_Slave_register_buffer[7] = (char*)(&WDG_REG_TIMER1)+1;
    USI_Slave_register_buffer[8] = (char*)(&WDG_REG_TIMER2);
	USI_Slave_register_buffer[9] = (char*)(&WDG_REG_TIMER2)+1;

  WDG_REG_TIMER0 = WDG_DEFAULT_TIMER0;
  WDG_REG_TIMER1 = WDG_DEFAULT_TIMER1;
  WDG_REG_TIMER2 = WDG_DEFAULT_TIMER2;
  timer0 = WDG_REG_TIMER0;
  timer1 = WDG_REG_TIMER1;
  timer2 = WDG_REG_TIMER2;
  timer3 = WDG_DEFAULT_TIMER3;
  WDG_REG_STATUS = WDG_STATUS_IDLE;
  sei();
}

//
// W przerwaniach odliczamy czasu
// przerwanie jest wywoływane raz na 100ms

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
    if (timer0 > 0 ) {timer0--;}
    if (timer1 > 0 ) {timer1--;}
    if (timer2 > 0 ) {timer2--;}

    TCNT1 = 63974;
}

//
// Wyłacz zasilanie na wszystkich liniach
//
void Call_PowerDown(void) {

    PORTB |= (1 << PB3) | (1 << PB4);
}

void Call_PowerUp(void) {

    PORTB &= ~((1 << PB3) | (1 << PB4));

}



int main(void) {

    int command;

    setup();

    while (1) {

// Jeśli wpisano coś do rejestru COMMAND, to wykonaj odpowiednią procedurę.

        if ( WDG_REG_COMMAND != WDG_CMD_NOP ) {
            command = WDG_REG_COMMAND;
            switch (command) {

// Pierwsze polecenie, to WDG_REG_FEED, które odświeża licznik
// Licznkiem, który jest zmniejszany to timer0.
// WDG_REG_TIMER0, to wartość programowana przez użytkownika

                case WDG_CMD_FEED:
                    WDG_REG_COMMAND = WDG_CMD_NOP;
                    timer0 = WDG_REG_TIMER0*10;
                    break;
            }
        }

//
// Jeżeli watchdog jest włączony, to okresowo zmniejszaj licznik.
// Jeśli licznik się wyzeruje, to reboot.
//
        if ( WDG_REG_STATUS && WDG_STATUS_ACTIVE != 0 ) {

// Sprawdź czy timer0 został wyzerowany.
// Jeśli tak to WDG_CMD_REBOOT

            if ( timer0 == 0 ) {
                WDG_REG_STATUS = WDG_STATUS_REBOOTING;
								timer2 = WDG_DEFAULT_TIMER2;
								Call_PowerDown();
						}
        }
// Jeżeli status jest równy WDG_STATUS_REBOOTING, to sprawdź czy timer2
// został wyzerowany. Jesli tak, to przywróć zasilanie.

        if (WDG_REG_STATUS && WDG_STATUS_REBOOTING != 0 ) {
            if ( timer2 == 0 ) {
							  WDG_REG_STATUS = WDG_STATUS_ACTIVE;
								timer0 = WDG_DEFAULT_TIMER0;
								Call_PowerUp();
						}

				}

//
// Jednostką czasu jest jedna sekunda.
//
  //      delay(1000);
		}

}
