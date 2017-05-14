/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware libraries */
#include <display.h>
#include <mbed.h>

/* Main elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int 	lastscore;
int   lives;
int 	tickCt;
struct ship player;

float Dt = 0.01f;

Ticker model, view, controller, timer;

bool paused = true;

// increments tickCt every second and rewards score
void timerHandler() {
	if (!paused) {
		tickCt++;
		elapsed_time = tickCt;
		score = tickCt;
	}
}

void initialise_game() {
	  minitialise();
	  ainitialise();
    init_DBuffer();
    
	  // tickers
    view.attach( draw, 0.025);
    model.attach( physics, Dt);
    controller.attach( controls, 0.1);
	  timer.attach(&timerHandler, 1);
    
	  // Game states
    lives = 5;
	  tickCt = 0;
	  score = 0;
	  elapsed_time = 0;
	
	  // Ship
		player.p.x = 260; player.p.y = 160;
		player.v.x = 0;	player.v.y = 0;
		player.loselife = 0;
    
    paused = true;
}

int main()
{
		initialise_game();
    
    while(true) {
        while( !paused ){
					while(lives == 0) {
						lastscore = score;
						reset();// restart the game
						initialise_game();
					}
        }
        
    }
}
