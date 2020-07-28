/**
@file ballphysics.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the module required for any game interactions with the ball. For
example, the current ball direction and row is set here.
*/

#include "system.h"
#include "ballphysics.h"
#include "led.h"
#include "ir_uart.h"
#include "ledmatrix.h"
#include "pacer.h"
#include "tinygl.h"
#include <stdlib.h>

// The total 'score' ie. amount of times ball goes out of range
#define FINAL_SCORE 4


/* returns 1 or 2 randomly for direction */
int randomBounceDirection (void)
{
    return 1 + rand() % 2;
}

/* This function detects what 'state' the ball should be in next, given its position, the paddle
position, and direction. 0 - 3 indicates it is returning a ball direction, returning 4 means we have
a crossover of boards. */
int detect_ball_state (int ball_row, int ball_col, int ball_dir, int paddle_pos)
{
    // returns the ball direction
    // returns 4 if we need to swap the ball onto other board
    if (ball_col == 3)
    { //column 3 is right before paddle; check for collision


        /* This is for when the ball bounces back from both paddle and wall. CORNER*/
        if (ball_dir == 3 && ball_row == 0 && paddle_pos == 0)
        {
            //bounce back where it came from
            return 1;
        }

        if (ball_dir == 0 && ball_row == 6 && paddle_pos == 4)
        { // ball is bouncing back from corner
            return 2;
        }



        if (ball_dir == 3)
        {
            //check if we are colliding with paddle
            if (ball_row >= paddle_pos + 1 && ball_row <= paddle_pos + 3)
            {
                //colliding
                // return random direction
                return randomBounceDirection();
            }


        }
        else if (ball_dir == 0)
        {
            if (ball_row >= paddle_pos - 1 && ball_row <= paddle_pos + 1)
            { //checking for paddle collision
                //colliding
                return randomBounceDirection();
            }

            if (ball_row == 6)
            {//we are at last row ie. colliding with wall
                return 3;
            }
        }
    }

    if (ball_dir == 3)
    {
        if (ball_row == 0)
        {//we are in first row colliding with wall;
            return 0;
        }
    }
    else if (ball_dir == 2)
    {
        if (ball_row == 0)
        {//we are in first row colliding with wall;
            return 1;
        }
    }
    else if (ball_dir == 1)
    {
        if (ball_row == 6)
        {//we are at last row ie. colliding with wall
            return 2;
        }
    }
    else if (ball_dir == 0)
    {
        if (ball_row == 6)
        {//we are at last row ie. colliding with wall
            return 3;
        }
    }

    /* This is for single player PONG! only!! Included for fun. */
    /*if (ball_col == 0)
    {

        if (ball_row == 6)
        {
            return 3;
        }
        else if (ball_row == 0)
        {
            return 0;
        }

        if (ball_dir  == 2)
        {
            led_on();
            return 3;
        }
        else if (ball_dir == 1)
        {
            led_on();
            return 0;
        }
    }*/

    // detecting a crossover of the ball
    if (ball_col == 0 && (ball_dir == 2 || ball_dir == 1))
    {
        return 4; //4 code for crossover
    }


    return ball_dir;
}

/* This function sets the value of ball_row and ball_col variables (by pointers!) according to ball direction */
void setBallPosition(int ball_dir, int* ball_row, int* ball_col)
{
    if (ball_dir == 0)
    {
        *ball_row = *ball_row + 1;
        *ball_col = *ball_col + 1;
    }
    else if (ball_dir == 1)
    {
        *ball_row = *ball_row + 1;
        *ball_col = *ball_col - 1;
    }
    else if (ball_dir == 2)
    {
        *ball_row = *ball_row - 1;
        *ball_col = *ball_col - 1;
    }
    else if (ball_dir == 3)
    {
        *ball_row = *ball_row - 1;
        *ball_col = *ball_col + 1;
    }
}

/* Determines the new position and direction of the ball on the new board by its old position and direction.
The new position and direction is stored in an int (first 4 bits = row, last 4 bits = direction)
NB: new col position is just 0.  */
int determineCrossoverPositionAndDirection (int ball_row, int ball_dir)
{
    int new_ball_row, new_ball_dir;
    int bounced = 0;
    if (ball_dir == 1)
    {
        if (ball_row == 0)
        {
            led_on('b');
            bounced = 1;
            new_ball_dir = 3;
            new_ball_row = 1;
        }
        else
        {
            new_ball_row = ball_row - 1;
        }
    }
    else
    {
        if (ball_row == 6)
        {
            led_on('b');
            bounced = 1;
            new_ball_dir = 0;
            new_ball_row = 5;
        }
        else
        {
            new_ball_row = ball_row + 1;
        }
    }

    new_ball_row = 6 - new_ball_row;
    if (!bounced)
    {
        if (ball_dir == 2)
        {
            new_ball_dir = 0;
        }
        else //ball_dir == 1
        {
            new_ball_dir = 3;
        }
    }

    int message = new_ball_row << 4 | new_ball_dir;

    return message;
}

/** Check if the game is ended by the ball position, score, etc. Reset these variables (via pointers) if so, and increment score. */
int checkGameOrRoundEnded (int* ball_col, int* my_score, int* ball_row, int* ball_dir)
{
    // return 0 if game continues, 1 if game ends.
    if (*ball_col == LEDMAT_COLS_NUM+1)
    {
        // this looks like its breaking but its detecting for when this player has lost!
        *my_score = *my_score + 1;

        // we will add onto our own 'score' but this will actually be the other player's score... 
        if (*my_score >= FINAL_SCORE)
        {
            return 1;
        }
        *ball_row = rand() % LEDMAT_ROWS_NUM; // randomly generates row position of newly-spawned ball
        *ball_col = 0;
        *ball_dir = rand() % 2 * 3; // randomly generates direction, either 0 or 3 (but always going towards losing player)
        // reset to original position
        
        return 2; // special code to indicate end of round
    }
    return 0;
}
