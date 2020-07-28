/**
@file ledmatrix.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the module for functions to do with displaying patterns and symbols
on the LED matrix, eg. displaying paddle and ball functions are here.
*/

#include <stdint.h> //this is required for the uint8_t type in ledmatrix.h
#include "ledmatrix.h"
#include "pio.h"
#include "navswitch.h"
#include "pacer.h"

static uint8_t prev_column = 0;

/** Define PIO pins driving LED matrix columns.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

/** Define the bitmaps required for paddle position */
static const uint8_t playerPosition[] =
{
    //This defines the paddle display for each position
    7,14,28,56,112
};

/** Updates the paddle position and calls display column to display the paddle. */
int update_paddle(int position)
{
    //puts the paddle in the correct position according to navswitch presses
    if (navswitch_push_event_p (NAVSWITCH_NORTH) && position > 0)
    {
        position--;
    }
    else if (navswitch_push_event_p (NAVSWITCH_SOUTH) && position < 4)
    {
        position++;
    }
    display_column (playerPosition[position], 4, prev_column);
    pacer_wait();
    return position;
}

/** sets up the led matrix by initialising all LEDs to low */
void ledmat_setup (void)
{
    pio_config_set(cols[0], PIO_OUTPUT_HIGH);
    pio_config_set(cols[1], PIO_OUTPUT_HIGH);
    pio_config_set(cols[2], PIO_OUTPUT_HIGH);
    pio_config_set(cols[3], PIO_OUTPUT_HIGH);
    pio_config_set(cols[4], PIO_OUTPUT_HIGH);
    pio_config_set(rows[0], PIO_OUTPUT_HIGH);
    pio_config_set(rows[1], PIO_OUTPUT_HIGH);
    pio_config_set(rows[2], PIO_OUTPUT_HIGH);
    pio_config_set(rows[3], PIO_OUTPUT_HIGH);
    pio_config_set(rows[4], PIO_OUTPUT_HIGH);
    pio_config_set(rows[5], PIO_OUTPUT_HIGH);
    pio_config_set(rows[6], PIO_OUTPUT_HIGH);
}
/** sets all led matrix to low voltage ie. off */
void reset_matrix (void)
{
    int i = 0;
    while (i < LEDMAT_ROWS_NUM) 
    {
        pio_output_high(rows[i]);
        i++;
    }
    i = 0;
    while (i < LEDMAT_COLS_NUM) 
    {
        pio_output_high(cols[i]);
        i++;
    }
}
/** displays bitmap for paddle given according to the column inputted */
void display_column (uint8_t row_pattern, uint8_t current_column, uint8_t prev_column)
{
    int i = 0;
    while (i < LEDMAT_ROWS_NUM) 
    {
        if ((row_pattern >> i) & 1)
        {
            pio_output_low(rows[i]);
        }
        else
        {
            pio_output_high(rows[i]);
        }
        i++;
    }
    i = 1;
    while (i < LEDMAT_COLS_NUM) 
    {
        pio_output_high(cols[i]);
        i++;
    }
    pio_output_high(cols[prev_column]);
    prev_column = current_column;
    pio_output_low(cols[current_column]);

}

/** displays ball on led matrix */
void display_ball (uint8_t row_pos, uint8_t col_pos)
{
    pio_output_low(rows[row_pos]);
    pio_output_low(cols[col_pos]);
}
