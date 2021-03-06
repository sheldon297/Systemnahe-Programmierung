/*
 * 2PotiAnalogRead.c
 *
 * Created: 25.04.2020 15:58:00
 * Author : Denis Denlger, Maximilian K�rschner, David Schader
 * Description: Read analog value from two potentiometers (version 1)
 */ 

/* includes */
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>					// needed for uint16_t
#include <avr/interrupt.h>
#include "serial.h"
#include "adc.h"

/* global variables*/
volatile uint16_t ADCvalue;    		// Global variable, set to volatile if used with ISR

/* function implementations */
void echo(){
	usart_send(usart_receive());
}

int main(void)
{
	adc_init_10();	
	usart_init(echo);
	
	while (1)
	{
		// main loop
	}
}


ISR(ADC_vect)
{
	cli();
	
	char buffer[10];
	uint16_t tmp;           // temp register for storage of misc data

	tmp = ADMUX;            // read the value of ADMUX register
	tmp &= 0x0F;            // AND the first 4 bits (value of ADC pin being used)

	ADCvalue = ADCH;        // read the sensor value

	if (tmp == 0){			//Poti at A0
		//concatenate HIGH Bits (ADC9 ADC8) to LOW Bits
		ADCvalue = ADCL | (ADCH << 8);
		//convert integer to string
		itoa(ADCvalue, buffer, 10);
		usart_send_string("Poti - A0: ");
		usart_send_string(buffer);
		ADMUX++;            // add 1 to ADMUX to go to the next sensor
	}
	 
	else if (tmp == 1){			//Poti at A1
		//concatenate HIGH Bits (ADC9 ADC8) to LOW Bits
		ADCvalue = ADCL | (ADCH << 8);
		//convert integer to string
		itoa(ADCvalue, buffer, 10);
		usart_send_string("   Poti - A1: ");
		usart_send_string(buffer);
		usart_send_string("\n\r");
		ADMUX &= 0xF8;      // clear the last 4 bits to reset the mux to ADC0
	}
	 
	sei();
}
