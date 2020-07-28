/**
@file led.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the module to do with initialising and setting the state of the blue LED.
*/

#include <stdint.h> //this is required for the uint8_t type in led.h
#include <avr/io.h>
#include "system.h"
#include "led.h"


/** Turn LED1 on.  */
void led_on (char colour)
{
	if (colour == 'b')
	{
    	PORTC |= BIT(2);
    }
    else if (colour == 'g')
    {
    	PORTD |= BIT(4);
    }
    else if (colour == 'r')
    {
    	PORTD |= BIT(5);
    }
}


/** Turn LED1 off.  */
void led_off (char colour)
{
	if (colour == 'b')
	{
    	PORTC &= ~BIT(2);
    }
    else if (colour == 'g')
    {
    	PORTD &= ~BIT(4);
    }
    else if (colour == 'r')
    {
    	PORTD &= ~BIT(5);
    }
}


/** Initialise LED1.  */
void led_init (void)
{
    DDRC |= BIT(2);
    DDRD |= BIT(4);
    DDRD |= BIT(5);
}

