/**
@file pacer.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the module to do with pacing the main gmae loop by waiting for small amounts of time, 
allowing patterns to be formed without turning on LEDs in the same column/row unintentionally.
*/

#include <avr/io.h>
#include "pacer.h"

static uint16_t pacer_period;

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency)
{
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TCCR1C = 0x00;
    pacer_period = 1 / (0.000128*pacer_frequency);
}


/* Pace a while loop.  */
void pacer_wait (void)
{
     while (TCNT1 < pacer_period)     
     {
         
     }
     TCNT1 = 0;
}

/* Pace a loop for a set frequency. */
void pacer_waitfor (uint16_t pacer_wait_frequency)
{
    uint16_t pacer_wait_period = 1 / (0.000128 * pacer_wait_frequency);
    while (TCNT1 < pacer_wait_period)     
    {
         
    }
    TCNT1 = 0;
}

/* Wait for a set amount of seconds */
void pacer_waitfor_seconds (int time)
{
    int count = 0;
    while (count < time)
    {
        pacer_waitfor(1);
        count++;
    }
}
