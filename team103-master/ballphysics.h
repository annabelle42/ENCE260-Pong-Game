/**
@file ballphysics.h
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the header file for the ballphysics module.
*/

#ifndef BALLPHYSICS_H
#define BALLPHYSICS_H

/* returns 1 or 2 randomly for direction */
int randomBounceDirection (void);

/** Returns ball direction (from 0 -> 3) or a special code 4 for swapping boards.  */
int detect_ball_state (int ball_row, int ball_col, int ball_dir, int paddle_pos);

/** sets the value of ball_row and ball_col variables (by pointers!) according to ball direction */
void setBallPosition(int ball_dir, int* ball_row, int* ball_col);

/** Determines the new position and direction of the ball on the new board by its old position and direction */
int determineCrossoverPositionAndDirection (int ball_row, int ball_dir);

/** Check if the game is ended by the ball position, score, etc. Reset these variables (via pointers) if so, and increment score. */
int checkGameOrRoundEnded (int* ball_col, int* my_score, int* ball_row, int* ball_dir);

#endif