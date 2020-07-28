/**
@file ir_send_receive.h
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the header file for the ir_send_receive module.
*/

#ifndef IR_SEND_RECEIVE_H
#define IR_SEND_RECEIVE_H


/** Checks for confirmation of successful send by ir  */
void check_if_sucessful_send (char character);

/** Sends a character by ir */
void send_message (char character);

/** Sends an acknowledgement message */
void send_ack_message (void);

/** check the IR for a specific character, return 1 if matches, 0 if not */
int checkIRfor(char character);

/** read the IR signal and return whatever character is detected */
char readIR(void);

#endif