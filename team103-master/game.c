/**
@file game.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the main game function. All main control happens here.
The game control functions are located here.
*/

#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "ledmatrix.h"
#include "led.h"
#include "pacer.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include "ballphysics.h"
#include "ir_send_receive.h"
#include "buzzer.h"
#include "../../extra/tweeter.h"
#include "../../extra/mmelody.h"

#define PACER_RATE 100
#define TINYGL_RATE 500
#define MESSAGE_RATE 20
#define NOSIGNAL_CHARACTER 'A'
#define START_CHARACTER 0x66
#define FINISH_CHARACTER 0x77
#define INITIAL_CHARACTER 0x67
#define END_OF_ROUND_CHARACTER 0x76

#define INT_MAX 32000
#define TRUE 1
#define FALSE 0
#define WON 1
#define LOST 0

#define GET_ROW(X) X >> 4
#define GET_DIR(X) X & 0X0F
#define COUNT_LIMIT 10
#define COUNT_LIMIT_DECREMENT 2
#define MAX_COUNT_LIMIT 4
#define WIN_TUNE 1
#define LOSE_TUNE 2
#define ROUND_WAIT 2200

#define INITIAL_PADDLE_POSITION 2
#define INITIAL_BALL_ROW 3
#define INITIAL_BALL_COL 0
#define INITIAL_BALL_DIR 0

/** Initialisation and setup of EVERYTHING */
void all_init (void)
{
	ir_uart_init();
    system_init ();
    navswitch_init ();
    ledmat_setup();
    led_init();
    pacer_init(PACER_RATE);
    tinygl_init (TINYGL_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
}

/** Initial start of game, displaying instructions and waiting for button push */
void startGame (int* ball_displaying)
{
	tinygl_text ("   PONG! PRESS JOYSTICK TO BEGIN");
	while(!navswitch_push_event_p(NAVSWITCH_PUSH))
    {
        // wait for the button to be pressed to exit loop, in the meantime display instructions
        pacer_waitfor(TINYGL_RATE);
        navswitch_update ();
        tinygl_update ();
        if (checkIRfor(INITIAL_CHARACTER))
        {
        	// if we get an IR signal telling us the other board has pressed button, we are player 2
        	// and so the ball does not display at first
        	*ball_displaying = FALSE;
        }
    }

    if (*ball_displaying)
    {
    	// if the button has been pushed and we have not received an IR signal yet, ball displaying is true
    	// and we display a waiting message on the screen while waiting for player 2's signal to start.
    	ir_uart_putc(INITIAL_CHARACTER);
    	tinygl_text(" WAITING...");
    	while (!checkIRfor(START_CHARACTER))
	    {
	    	pacer_waitfor(TINYGL_RATE);
	    	tinygl_update();
	    }
    }
    else
    {
    	// if we are player 2 then we send a start message to the other board when button is pushed
    	ir_uart_putc(START_CHARACTER);
    }
    // wait for IR signal to go away before beginning main loop!
}

/** Function for the end of game - state TRUE indicates win, FALSE is loss */
void gameEnded (int state)
{
	if (state == WON)
	{
		// execute the winning buzzer if the other board detects losing
		reset_matrix();
		led_on('g');
		execute_buzzer(WIN_TUNE);
		
		while (TRUE)
		{
			// This text is not showing after the buzzer for some reason.
			tinygl_update();
			pacer_waitfor(TINYGL_RATE);
		}
	}
	else if (state == LOST)
	{
		// set text for displaying losing message
    	tinygl_text_speed_set(MESSAGE_RATE);
    	tinygl_text("YOU HAVE LOST");

    	// send an IR message to the other board signaling end of game
    	ir_uart_putc(FINISH_CHARACTER);

    	// red LED on to signal loss, play sound
    	led_on('r');
    	execute_buzzer(LOSE_TUNE);

		// NB: At the moment the buzzer seems to jam the screen. Fix if time allows
		while(TRUE)
		{
			pacer_waitfor(TINYGL_RATE);
			tinygl_update ();
			// wait for the button to be pressed to exit loop, in the meantime display instructions
			// END OF GAME.
		}
	}
}

/** Code for the end of the round, state depending on winner/loser */
void endOfRound (int state, int* count_limit)
{
	char ledIdentifier;
	if (state == WON)
	{
		ledIdentifier = 'g';
	}
	else
	{
		ledIdentifier = 'r';
	}

	// we have finished round: turn on green/red LED, display next round message
	led_on(ledIdentifier);

	reset_matrix();
	int round_time_counter = 0;
	tinygl_text_speed_set(MESSAGE_RATE * 2);
	tinygl_text("   NEXT ROUND");

	while (round_time_counter < ROUND_WAIT)
	{
		// display next round message for a set time
		pacer_waitfor(TINYGL_RATE);
		tinygl_update();
		round_time_counter++;
	}

	led_off(ledIdentifier);

	tinygl_text_speed_set(MESSAGE_RATE);

	if (*count_limit > MAX_COUNT_LIMIT)
	{
		// decrement the count limit to speed up ball
		// (as long as we are within limit! so ball doesn't get ridiculously fast)
		*count_limit = *count_limit - COUNT_LIMIT_DECREMENT;
	}

}

int main (void)
{
	// initialise the paddle position, ball row, ball column, ball direction, loop counter, score.
    int paddle_position = INITIAL_PADDLE_POSITION;
    int ball_row = INITIAL_BALL_ROW;
    int ball_col = INITIAL_BALL_COL;
    int ball_dir = INITIAL_BALL_DIR;
    int previous_ball_dir = ball_dir;
    // ball_dir: 0, 1, 2, 3
    int count = 0;
    int ball_displaying = TRUE;
    int my_score = 0;
    int level_increment = 1;
    int count_limit = 20;

    all_init();
    
    startGame(&ball_displaying);

    /* MAIN GAME CONTROL LOOP BEGINS */
    while (TRUE)
    {
    	// update the joystick and paddle position - done every iteration!
        navswitch_update ();
        paddle_position = update_paddle(paddle_position);

        if (ball_displaying)
        {
        	// if this board 'has' the ball at the moment, then display ball and perform ball calculations etc
        	previous_ball_dir = ball_dir;
	        ball_dir = detect_ball_state(ball_row, ball_col, ball_dir, paddle_position);

	        reset_matrix();

	        // CHECK FOR FINISH OF GAME
	        int endedState = checkGameOrRoundEnded(&ball_col, &my_score, &ball_row, &ball_dir);
	        
	        if (endedState == 1)
	        {
	        	// end of game
	        	gameEnded(LOST);
	        }
	        else if (endedState == 2)
	        {
	        	// end of round
	        	ir_uart_putc(END_OF_ROUND_CHARACTER);
	        	endOfRound(LOST, &count_limit);
	        }

	        // reset LED matrix and display the ball
        	reset_matrix();
        	display_ball(ball_row, ball_col);

        	// every count_limit times the loop runs, change the ball position
        	// this allows us to change the 'speed' ie. refresh rate of the ball 
        	if (count % count_limit == 0)
	        { 
	        	if (ball_dir == 4)
				{
					// the ball is supposed to go off of the screen here; special state 4 tells us this
					// store the previous ball direction as the current one stored is 4 which is a special code
					ball_dir = previous_ball_dir;

					// calculate the integer to send and seng it
					char outgoing = determineCrossoverPositionAndDirection(ball_row, previous_ball_dir);
        			send_message(outgoing);

        			// toggle the ball displaying state
        			ball_displaying = FALSE;
				}
				else
				{
					// If the ball is NOT shifting boards, then set the new ball position.
	        		setBallPosition(ball_dir, &ball_row, &ball_col);
	        	}
	        }

	        // pacer wait to allow ball display to show!
		    pacer_wait();
		    reset_matrix();
		}
		else // if this board is NOT displaying the ball currently
		{
			uint8_t incoming_character = 0;

			if (ir_uart_read_ready_p ())
			{
				// if the IR sensor has new data being input, then take it and determine what to do based on communication
				incoming_character = (uint8_t) (ir_uart_getc());
				if (incoming_character == FINISH_CHARACTER)
				{
					// other player has lost game, we got signal
					gameEnded(WON);
				}
				else if (incoming_character == END_OF_ROUND_CHARACTER)
				{
					// other player has lost a round, we have gotten signal
					endOfRound(WON, &count_limit);
				}
				else
				{
					// the other board is sending position and direction data on ball; extract it
					reset_matrix();
					ball_row = GET_ROW(incoming_character);
					ball_dir = GET_DIR(incoming_character);
					display_ball(ball_row, 0);

					// toggle the ball displaying state
					ball_displaying = TRUE;
				}
			}

			// pace the loop
			pacer_wait();

			
		}

		// blue led is just for debugging purposes
		// reset all LEDs
		led_off('g');
		led_off('r');
		led_off('b');

        count += level_increment;
        // count is incremented by a variable level_increment
        // in future implementation this increment could be changed

        if (count >= INT_MAX)
        { // count wraps around to avoid integer limit
        	count = 0;
        }
	}
}
