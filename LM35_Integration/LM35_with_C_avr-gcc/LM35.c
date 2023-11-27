/*This code is to use an adc and timers to utilize the Voltage based LM35 sensor
I have coded this in using avr-gcc but the code will work on the arduino IDE
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "USART.h"


/*Blinking LED COnfiguration*/
#define LED_PORT 	PORTB
#define PORT_PINS 	0b00000001
#define LED_DDR		DDRB

/*Change the REF VCC based on the measured 
value if you want accurate sensor reading */
#define REF_VCC 5.053   
// Clock frequency and prescaler
#define CLOCK_FREQ 1000000UL  // 1 MHz
#define PRESCALER 1024

// To calculate compare value
// OCR1A = (time(s) * clock Freq.)/prescaler
// Calculate Timer compare frequency (TCF) and period (TCP)
#define TCF (CLOCK_FREQ / PRESCALER)
#define TCP (1.0 / TCF)

// Define timing options in milliseconds
#define BLINK_INTERVAL_250MS (250 / TCP)
#define BLINK_INTERVAL_500MS (500 / TCP)


void initADC(void) {
ADMUX |= (0b00001111 & PC0);  			/* set mux to ADC0*/
ADMUX |= (1 << REFS0);					/* reference voltage on AVCC*/
ADCSRA |= (1 << ADPS1) | (1 << ADPS2);	/* ADC clock prescaler /64*/
ADCSRA |= (1 << ADEN);					/* enable ADC*/
}

static inline void initTimer1(void) {
TCCR1A |= (1 << WGM11);					/* CTC mode */
TCCR0B |= (1 << CS10) | (1 << CS11);	/* CPU clock / 64 */
TIMSK1 |= (1 << OCIE1A); 				/*Enable Inturrupt for Output compare match A*/
sei();									//Enable Global inturrupt

}

ISR(TIMER1_COMPA_vect)
{
	LED_PORT ^= PORT_PINS; // TOGGLE THE LEDs  
}

int main(void) 
{
 float voltage;
 float temperature;
// -------- Inits --------- //
LED_DDR = PORT_PINS;
initUSART();
initTimer1();
initADC();
// ------ Event loop ------ //
while (1) {
voltage = ADC * REF_VCC / 1024;
temperature = voltage * 100;

printFloat(temperature);
printString("degree Celcius")


if(temperature<30)
{

	OCR1A = BLINK_INTERVAL_250MS;       
    /*Setting the Output Compare Register A value so that the interval is  250ms*/
}
else
{
	OCR1A = BLINK_INTERVAL_500MS;
    /*Setting the Output Compare Register A value so that the interval is  500ms*/
}
}
return (0);
}