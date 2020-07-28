/**
@file ledmatrix.h
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the header file for the ledmatrix module.
*/

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

/** Updates the paddle position and calls display column to display the paddle. */
int update_paddle(int position);

/** sets up the led matrix by initialising all LEDs to low */
void ledmat_setup (void);

/** sets all led matrix to low voltage ie. off */
void reset_matrix (void);

/** displays bitmap for paddle given according to the column inputted */
void display_column (uint8_t row_pattern, uint8_t current_column, uint8_t prev_column);

/** displays ball on led matrix */
void display_ball (uint8_t row_pos, uint8_t col_pos);

#endif
