/**
@file pacer.h
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the header file for the pacer module.
*/

#ifndef PACER_H
#define PACER_H

#include "system.h"

/* Initialise the pacer module.  */
void pacer_init (uint16_t pacer_frequency);


/* Pace a while loop.  */
void pacer_wait (void);

/** wait for a set frequency */
void pacer_waitfor (uint16_t pacer_wait_frequency);

/* Wait for a set amount of seconds */
void pacer_waitfor_seconds (int time);

#endif //PACER_H

