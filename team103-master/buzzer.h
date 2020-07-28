/**
@file buzzer.h
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the header file for the buzzer module.
*/

#ifndef BUZZER_H
#define BUZZER_H

/* Initialises the 'tweeter' task */
void tweeter_task_init (void);

/* Updates the melody */
void tune_task (__unused__ void *data);

/** Plays either winning or losing melody, given the state */
void execute_buzzer (int given_tune_state);

#endif