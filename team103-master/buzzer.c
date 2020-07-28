/**
@file buzzer.c
@author Annabelle Ritchie and Braden Smitheram
@date 16 Oct 2017
A PONG! game source file.
This is the buzzer module file, used for playing a sound signaling victory or defeat in game.
NB: Largely based off squeak0.c, found in apps/squeak0 folder. We do NOT claim full responsibility for this.
The changes made are specifically to do with the tunes played, and adding in a state variable
to differ between winning and losing sounds.
Original author: M.P. Hayes, 16 Sep 2011
*/

#include "system.h"
#include "pio.h"
#include "../../utils/task.h"
#include "../../extra/tweeter.h"
#include "../../extra/mmelody.h"
#include "tinygl.h"


/** Connect piezo tweeter to outermost pins of UCFK4 P1 connector.  */
#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)

/** Define polling rates in Hz.  */
#define TWEETER_TASK_RATE 20000

#define TUNE_TASK_RATE 500

#define TUNE_BPM_RATE 200


static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;


///TUNES and cossing system

static const char tune_win[] =
{
#include "win.mmel"
"    >"
};

static const char tune_lose[] =
{
#include "lose.mmel"
"    >"
};

///Nothing = 0
///Win = 1
///Lose = 2
int tune_state = 0;



/* Initialises the 'tweeter' task */
static void tweeter_task_init (void)
{
    static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);

    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);

    pio_config_set (PIEZO_PIO, PIO_OUTPUT_LOW);
}

/* Updates the 'tweeter' */
static void tweeter_task (__unused__ void *data)
{
    pio_output_set (PIEZO_PIO, tweeter_update (tweeter));
}

/* Initialises the tune task, determines which tune to play based on state */
static void tune_task_init (void)
{
    melody = mmelody_init (&melody_info, TUNE_TASK_RATE,
               (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);



    ///play selected tune
    if (tune_state == 1)
    {
        mmelody_play (melody, tune_win);
    }
    else if (tune_state == 2)
    {
        mmelody_play (melody, tune_lose);
    }
}

/* Updates the melody */
static void tune_task (__unused__ void *data)
{
    tinygl_update();
    mmelody_update (melody);
}

/** Plays either winning or losing melody, given the state */
void execute_buzzer (int given_state)
{
    tune_state = given_state;
    /** There are two tasks:
         the tweeter task plays the notes
         the tune tasks sequences the tune.
    */
    task_t tasks[] =
    {
        {.func = tweeter_task, .period = TASK_RATE / TWEETER_TASK_RATE},
        {.func = tune_task, .period = TASK_RATE / TUNE_TASK_RATE},
    };
    if (given_state == 1)
    {
        // winning state
        tinygl_text("   YOU WON");
    }
    else
    {
        tinygl_text("   YOU LOST");
    }
    tweeter_task_init ();
    tune_task_init ();

    task_schedule (tasks, ARRAY_SIZE (tasks));
}
