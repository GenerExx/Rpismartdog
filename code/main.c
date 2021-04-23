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
 //       O5V   PB3  2|    |7  PB2  SCL
 //       O3V   PB4  3|    |6  PB1  LED
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

// ACTIVE - czyli o tego momentu jak timer0 osiągnie 0 to reboot.
#define WDG_STATUS_ACTIVE    0x01
#define WDG_STATUS_REBOOTING 0x02
#define WDG_STATUS_POWEROFF  0x04

#define WDG_CMD_NOP           0x00
#define WDG_CMD_BOOTCOMPLETE  0x01
#define WDG_CMD_KEEPALIVE     0x02
#define WDG_CMD_POWEROFF      0x03
#define WDG_CMD_LINE3_OFF     0x04
#define WDG_CMD_LINE3_ON      0x05
#define WDG_CMD_STOREDEFAULTS 0x06


// timer0 - główny zegar watchdoga
// timer1 - zegar mierzący jak długo zasilanie ma być wyłączone
// timer2 - zegar mierzący po jakim okresie włączyć zasilanie
// timer3 - zegar mierzący czas zwłoki po jakim WDG się aktywuje

#define WDG_DEFAULT_TIMER0   (unsigned int) 120
#define WDG_DEFAULT_TIMER1   (unsigned int) 10
#define WDG_DEFAULT_TIMER2   (unsigned int) 3600
#define WDG_DEFAULT_TIMER3   (unsigned int) 600

#define WDG_O5V_Line  PB3
#define WDG_O3V_Line  PB4


volatile short int WDG_REG_COMMAND;
volatile short int WDG_REG_CONFIG;
volatile short int WDG_REG_STATUS;
volatile short int WDG_REG_RESERVED;
volatile unsigned int WDG_REG_TIMER0;
volatile unsigned int WDG_REG_TIMER1;
volatile unsigned int WDG_REG_TIMER2;
volatile unsigned int WDG_REG_TIMER3;


unsigned int timer0, timer1, timer2, timer3;

void Load_Defaults(void) {

    WDG_REG_TIMER0 = WDG_DEFAULT_TIMER0;
    WDG_REG_TIMER1 = WDG_DEFAULT_TIMER1;
    WDG_REG_TIMER2 = WDG_DEFAULT_TIMER2;
    WDG_REG_TIMER3 = WDG_DEFAULT_TIMER3;
    timer0 = 0;
    timer1 = 0;
    timer2 = 0;
    timer3 = WDG_DEFAULT_TIMER3;

}

void Store_Defaults(void) {


}

void Timer0_Setup(void){
    cli();
// Clear registers
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
// 10.067654639175258 Hz (1000000/((96+1)*1024))
    OCR0A = 96;

// Dla 100Hz
    OCR0A = 155;
/*
    TCCR0A - Timer/Counter Control Register A
    7:6 - [COM0A] - Compare Match Output A mode
    5:4 - [COM0B] - Compare Match Output B Mode
    3:2 - Reserved
    1:0 - [WGM01,WGM00] Waveform Generation Mode

    Set CTC Mode
    WGM02, WGM01, WGM00 = 0 1 0 = CTC Mode - Top = OCRA;
    Registers:
    TCCR0A, TCCR0B
*/
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |=  (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
/*
    TCCR0B
    7:6 - [FOC0A, FOC0B] - Force Output Compare B
    5:4 - Reserved
    3   - [WGM02] - Waveform Generation mode
    2:0 - [CS] - Clock Select

    CS02, CS01, CS00 = 1 0 0 = Clkio/1024

    Set prescaler=1024
*/
//    TCCR0B &= ~(1 << CS01);
//    TCCR0B |= (1 << CS02) | (1 << CS00);

//  Prescaler = 64 - dla 100 Hz
    TCCR0B |= ( 1 << CS01 ) | ( 1 << CS00 );

// Output Compare Match A Interrupt Enable
    TIMSK |= (1 << OCIE0A);
    sei();
}

/*
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
// PB1 - 3.3 V
*/
void setup() {
	DDRB  = 0b00011000; //
	PORTB = 0b00000101; // internal pull-ups on
    Load_Defaults();
    WDG_REG_STATUS = 0x00;
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
    USI_Slave_register_buffer[10] = (char*)(&WDG_REG_TIMER3);
    USI_Slave_register_buffer[11] = (char*)(&WDG_REG_TIMER3)+1;
    WDG_REG_STATUS = 0;
    WDG_REG_COMMAND = 0;
    sei();
}

int LedOn;
int LedTimer;
int LedHTime, LedLTime;


ISR (TIMER0_COMPA_vect) {
    if (LedTimer > 0) {LedTimer--;}
    if (LedTimer == 0){
        LedOn = 1 - LedOn;
        PORTB ^= ( 1 << PB1 );
        if ( LedOn == 1 ) {
            LedTimer = LedHTime;
        } else {
            LedTimer = LedLTime;
        }
    }
}


void Timer1_Setup() {
  cli();
  // Clear registers
  TCNT1 = 0;
  TCCR1 = 0;

  // 1.0005763319672132 Hz (1000000/((243+1)*4096))
  OCR1C = 243;
  // interrupt COMPA
  OCR1A = OCR1C;
  // CTC
  TCCR1 |= (1 << CTC1);
  // Prescaler 4096
  TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK |= (1 << OCIE1A);
  sei();
}


ISR(TIMER1_COMPA_vect) {
    if (timer0 > 0 ) {timer0--;};
    if (timer1 > 0 ) {timer1--;};
    if (timer2 > 0 ) {timer2--;};
    if (timer3 > 0 ) {timer3--;};
}



void Exec_Line5_Down(void) { PORTB |= (1 << WDG_O5V_Line); }
void Exec_Line3_Down(void) { PORTB |= (1 << WDG_O3V_Line); }
void Exec_Line5_Up(void) { PORTB &= ~(1 << WDG_O5V_Line); }
void Exec_Line3_Up(void) { PORTB &= ~(1 << WDG_O3V_Line); }

void Exec_PowerDown(void) {
 //   Exec_Line3_Down();
 //   Exec_Line5_Down();
}

void Exec_PowerUp(void) {
 //   Exec_Line3_Up();
 //   Exec_Line5_Up();
}

int main(void) {

    int command;
    int temp;
    setup();
    Timer0_Setup();
    Timer1_Setup();
    LedOn = 0;
    LedHTime = 25;
    LedLTime = 25;

    while (1) {

        if ( WDG_REG_COMMAND != WDG_CMD_NOP ) {
            command = WDG_REG_COMMAND;
            switch (command) {

                case WDG_CMD_BOOTCOMPLETE:
                    timer3 = 0;
                    timer0 = WDG_REG_TIMER0;
                    WDG_REG_STATUS |= WDG_STATUS_ACTIVE;
                    break;

                case WDG_CMD_KEEPALIVE:
                    timer0 = WDG_REG_TIMER0;
                    break;

                case WDG_CMD_POWEROFF:
                    Exec_PowerDown();
                    timer2 = WDG_REG_TIMER2;
                    WDG_REG_STATUS |= WDG_STATUS_POWEROFF;
                    break;

                case WDG_CMD_LINE3_ON:
                    Exec_Line3_Up();
                    break;

                case WDG_CMD_LINE3_OFF:
                    Exec_Line3_Down();
                    break;

            }
            WDG_REG_COMMAND = WDG_CMD_NOP;
        }

/*
// Sprawdź czy to pierwsze uruchomienie, tj. status jest 0.
// Jeśli tak, to po wyzerowaniu timer3 ustaw status ACTIVE.
// Od tego momentu, zerowanie timer0 będzie powodowało restart.
*/
        if ( ( WDG_REG_STATUS & WDG_STATUS_ACTIVE ) != WDG_STATUS_ACTIVE ) {
            if ( timer3 == 0 ) {
                timer0 = WDG_REG_TIMER0;
                LedHTime = 3;
                LedLTime = 197;
                WDG_REG_STATUS |= WDG_STATUS_ACTIVE;
                PORTB ^= ( 1 << PB3 );
            }
        }
/*
// Sprawdź czy timer0 został wyzerowany.
// Jeśli tak to REBOOT, wyłacz MOSFETy i zacznij odliczać czas w timer1
*/

// BUG - Status ACTIVE and NOT REBOOTING

        if ( ( ( WDG_REG_STATUS & WDG_STATUS_ACTIVE ) == WDG_STATUS_ACTIVE ) &&
           ( ( WDG_REG_STATUS & WDG_STATUS_REBOOTING ) != WDG_STATUS_REBOOTING ) ) {
            if ( timer0 == 0 ) {
                timer1 = WDG_REG_TIMER1;
                LedHTime = 5;
                LedLTime = 5;
                WDG_REG_STATUS |= WDG_STATUS_REBOOTING;
				Exec_PowerDown();
				PORTB ^= ( 1 << PB3 );
			}
        }
/*
// Jeżeli mamy status REBOOTING to czekamy aż timer1 się wyzeruje
// i wtedy podnosimy zasilanie na obu liniach.
// Dodatkowo, ustawiamy timer3 i zerujemy status ACTIVE.
// Timer3 odlacza czas potrzebny na uruchomienie RPi.
*/

        if ( ( WDG_REG_STATUS & WDG_STATUS_REBOOTING ) == WDG_STATUS_REBOOTING ) {
            if ( timer1 == 0 ) {
                timer3 = WDG_REG_TIMER3;
                LedHTime = 25;
                LedLTime = 25;
                WDG_REG_STATUS &= ~(WDG_STATUS_REBOOTING | WDG_STATUS_ACTIVE);
                Exec_PowerUp();
                PORTB ^= ( 1 << PB3 );
            }
        }
/*
        if ( WDG_REG_STATUS && WDG_STATUS_POWEROFF != 0 ) {
            if ( timer2 == 0 ) {
                timer3 = WDG_REG_TIMER3;
                WDG_REG_STATUS &= ~(WDG_STATUS_POWEROFF | WDG_STATUS_ACTIVE);
                Exec_PowerUp();
            }
        }
*/

// Enter low power mode.
    //sei();
        //set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        //sleep_mode();
    //cli();
    }

}
