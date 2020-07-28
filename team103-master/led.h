/**
@file led.h
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the header file for the LED module.
NB: r is in PD4, g is in PD5
*/

#ifndef LED_H
#define LED_H

/** Turn an LED - either r, g, b - on.  */
void led_on (char colour);


/** Turn an LED off - either r, g, b.  */
void led_off (char colour);


/** Initialise all 3 LEDs  */
void led_init (void);
#endif
