/*
 * main.c
 *
 *  Created on: Apr 09, 2021
 *      Author: Pawel Tanski
 */

#define F_CPU 8000000UL

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


// timer0 - główny zegar watchdoga
// timer1 - zegar mierzący jak długo zasilanie ma być wyłączone
// timer2 - zegar mierzący po jakim okresie włączyć zasilanie
// timer3 - zegar mierzący czas zwłoki po jakim WDG się sam włączy

#define WDG_DEFAULT_TIMER0   30
#define WDG_DEFAULT_TIMER1   10
#define WDG_DEFAULT_TIMER2   3600
#define WDG_DEFAULT_TIMER3   120


volatile short int WDG_REG_COMMAND;
volatile short int WDG_REG_CONFIG;
volatile short int WDG_REG_STATUS;
volatile short int WDG_REG_RESERVED;
volatile unsigned int WDG_REG_TIMER0;
volatile unsigned int WDG_REG_TIMER1;
volatile unsigned int WDG_REG_TIMER2;

unsigned int timer0, timer1, timer2;

void setup() {

	DDRB  = 0b00000000; // all inputs
	PORTB = 0b00011010; // internal pull-ups on

	cli();
	USI_I2C_Init(0x33);
	USI_Slave_register_buffer[0] = (unsigned char*)(&WDG_REG_COMMAND);
	USI_Slave_register_buffer[1] = (unsigned char*)(&WDG_REG_CONFIG);
	USI_Slave_register_buffer[2] = (unsigned char*)(&WDG_REG_STATUS);
	USI_Slave_register_buffer[3] = (unsigned char*)(&WDG_REG_RESERVED);
  USI_Slave_register_buffer[4] = (unsigned char*)(&WDG_REG_TIMER0);
	USI_Slave_register_buffer[5] = (unsigned char*)(&WDG_REG_TIMER0)+1;
  USI_Slave_register_buffer[6] = (unsigned char*)(&WDG_REG_TIMER1);
	USI_Slave_register_buffer[7] = (unsigned char*)(&WDG_REG_TIMER1)+1;
  USI_Slave_register_buffer[8] = (unsigned char*)(&WDG_REG_TIMER2);
	USI_Slave_register_buffer[9] = (unsigned char*)(&WDG_REG_TIMER2)+1;

  WDG_REG_TIMER0 = WDG_DEFAULT_TIMER0 * 10;
  WDG_REG_TIMER1 = WDG_DEFAULT_TIMER1 * 10;
  WDG_REG_TIMER2 = WDG_DEFAULT_TIMER2 * 10;
  timer0 = WDG_REG_TIMER0;
  timer1 = WDG_REG_TIMER1;
  timer2 = WDG_REG_TIMER2;
  timer3 = WDG_DEFAULT_TIMER3 * 10;
  WDG_REG_STATUS = WDG_STATUS_IDLE;
  sei();
}


// W przerwaniach odliczamy czasu
// przerwanie jest wywoływane raz na 100ms

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	  if (timer0 > 0 ) {timer0--;}
    if (timer1 > 0 ) {timer1--;}
    if (timer2 > 0 ) {timer2--;}


    
    TCNT1 = 63974;
}



main {

    int command;

    setup();

    while (1) {

// Jeśli wpisano coś do rejestru COMMAND, to wykonaj odpowiednią procedurę.

        if ( WDG_REG_COMMAND != WGD_CMD_NOP ) {
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
        }
//
// Jeżeli watchdog jest włączony, to okresowo zmniejszaj licznik.
// Jeśli licznik się wyzeruje, to reboot.
//
        if ( WDG_REG_STATUS && WDG_STATUS_ACTIVE != 0 ) {


        }
//
// Jednostką czasu jest jedna sekunda.
//
        delay(1000)

    }
