/**
@file ir_send_receive.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the module for sending and receiving IR signals, including receiving and acknowledging signals.
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "led.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 100
#define MESSAGE_RATE 10

/** Checks for confirmation of successful send by ir  */
void check_if_sucessful_send (char character)
{
	// while loop check
	// 0 = False
	// 1 = True
	int check = 0;
	int count = 0;
	
	uint8_t check_message = 0;

	while (check == 0)
    {
		
		pacer_wait();
		
		if (count%20 == 0)
        { 
			ir_uart_putc (character);
		}
		
		
		if (ir_uart_read_ready_p ())
		{
			led_on('b');
			check_message =(uint8_t) ir_uart_getc ();
			if (check_message == 0xFF)
			{
				check = 1;
			}
			
		}
		
		count++;
	}
}

/** Sends a character by ir */
void send_message (char character)
{
	ir_uart_putc (character);
	//check_if_sucessful_send (character);
}

/** Sends an acknowledgement message */
void send_ack_message (void)
{
	uint8_t character = 0xFF;
	
	int count = 0;

	ir_uart_putc (character);
	
	count++;
}

/** check the IR for a specific character, return 1 if matches, 0 if not */
int checkIRfor(char character)
{
	if (ir_uart_read_ready_p())
	{
		if (ir_uart_getc() == character)
		{
			return 1;
		}
	}
	return 0;
}

/** read the IR signal and return whatever character is detected */
char readIR(void)
{
	if (ir_uart_read_ready_p())
	{
		return ir_uart_getc();
	}
	return 0;
}
