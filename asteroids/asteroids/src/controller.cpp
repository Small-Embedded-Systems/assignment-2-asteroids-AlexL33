/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"
#include "view.h"

// Joystick 5-way switch
typedef enum {left, down, right, up, centre} btnId_t;
DigitalIn joystick[] = {P5_0, P5_1, P5_4, P5_2, P5_3};
bool jsPrsdAndHld(btnId_t b);

/* Adds a bullet and fires*/
void fire() {
	missile_handler();
}

/* Changes the ships velocity and direction based on user input */
void controls(void)
{
	if ((jsPrsdAndHld(up) && jsPrsdAndHld(right))) { // NW OR UP+RIGHT
		player.v.x = -1; player.v.y = -1;
		player.heading = 1;
	}
	else if ((jsPrsdAndHld(right) && jsPrsdAndHld(down))) { // SW OR DOWN+RIGHT
		player.v.x = -1; player.v.y = 1;
		player.heading = 3;
	}
	else if ((jsPrsdAndHld(down) && jsPrsdAndHld(left))) { // SE OR DOWN+LEFT
		player.v.x = 1; player.v.y = 1;
		player.heading = 5;
	}
	else if ((jsPrsdAndHld(left) && jsPrsdAndHld(up))) { // NE or UP+LEFT
		player.v.x = 1; player.v.y = -1;
		player.heading = 7;
	}
	else if (jsPrsdAndHld(up)) { // N or UP
		player.v.y = -1;
		player.heading = 0;
	}
	else if (jsPrsdAndHld(right)) { // W OR RIGHT
		player.v.x = -1;
		player.heading = 2;
	}
	else if (jsPrsdAndHld(down)) { // S OR DOWN
		player.v.y = 1;
		player.heading = 4;
	}
	else if (jsPrsdAndHld(left)) { // E OR LEFT
		player.v.x = 1;
		player.heading = 6;
	}
	
	/* Starts the game from initial pause.
	   If the game is in play, fires a missile */
	else if (jsPrsdAndHld(centre)) {
		if (paused) {
			paused = false;
		}
		else if (!paused) {
			fire();
		}
	}
	
	else {	
		player.v.x = 0; player.v.y = 0;
	}
}


/* Definition of Joystick press capture function
 * b is one of JLEFT, JRIGHT, JUP, JDOWN - from enum, 0, 1, 2, 3
 * Returns true if this Joystick pressed, false otherwise.
 *
 * If the value of the button's pin is 0 then the button is being pressed,
 * just remember this in savedState.
 * If the value of the button's pin is 1 then the button is released, so
 * if the savedState of the button is 0, then the result is true, otherwise
 * the result is false. */
bool jsPrsdAndHld(btnId_t b) { // changed from pressed and released to held
	bool result = false;
	uint32_t state;
	static uint32_t savedState[5] = {1,1,1,1,1};
	state = joystick[b].read();
  if ((savedState[b] == 0) && (state == 0)) {
		result = true;
	}
	savedState[b] = state;
	return result;
}
