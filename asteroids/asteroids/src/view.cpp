/* Asteroids view
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();

const colour_t background = rgb(0,0,0);
const colour_t sidebar = rgb(30,60,60);


const coordinate_t shape[] = {
   {10,0}, {-5,5}, {-5,-5}
};

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}

/* Draw sidebar, containing elapsed time, score, 
	 lives, and previous score */
void draw_sidebar(int e_t, int sc, int li) {
	graphics->fillRect(0, 0, 480, 50, sidebar);
	  graphics->setTextColor(RED, sidebar);
		graphics->setCursor(20, 10);
			graphics->printf("CM0506");
		graphics->setCursor(2, 20);
			graphics->printf("Assignment 2");
		graphics->setCursor(10, 30);
			graphics->printf("Asteroids");
		graphics->setCursor(97, 10);
			graphics->printf("Game Time");
		graphics->setCursor(97, 20);
	    graphics->printf("elapsed:");
		graphics->setCursor(102, 35);
	    graphics->printf("%d", e_t);
		graphics->setCursor(185, 10);
	    graphics->printf("Score:");
		graphics->setCursor(200, 20);
	    graphics->printf("%d", sc);
		graphics->setCursor(250, 10);
		  graphics->printf("Lives:");
		graphics->setCursor(260, 20);
	    graphics->printf("%d", li);
		graphics->setCursor(420, 10);
		graphics->printf("Last Games");
		graphics->setCursor(420, 20);
			graphics->printf("Score:");
		graphics->setCursor(440, 30);
	    graphics->printf("%d", lastscore);
			
			if (paused) {
				graphics->setCursor(140, 90);
					graphics->setTextColor(GREEN);
					graphics->printf("Press+hold centre on joystick to begin");
			}
}

/* Draw ship based on heading and indicates a life 
	is lost on hit with asteroid by turning ship red*/
void draw_ship(ship player) {
	if (player.heading == 0) { 
		graphics->fillTriangle(player.p.x, player.p.y-4,
		player.p.x-2, player.p.y+2,
		player.p.x+2, player.p.y+2,
		GREEN);
				if (player.loselife == 1) {
			graphics->fillTriangle(player.p.x, player.p.y-4,
			player.p.x-2, player.p.y+2,
			player.p.x+2, player.p.y+2,
			RED);
			}
	}
	
	if (player.heading == 1) { 
		graphics->fillTriangle(player.p.x-3, player.p.y-3,
		player.p.x, player.p.y+3,
		player.p.x+3, player.p.y,
		GREEN);
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x-3, player.p.y-3,
				player.p.x, player.p.y+3,
				player.p.x+3, player.p.y,
				RED);
			}
	}

	if (player.heading == 2) { 
		graphics->fillTriangle(player.p.x-4, player.p.y,
		player.p.x+2, player.p.y+2,
		player.p.x+2, player.p.y-2,
		GREEN);			  
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x-4, player.p.y,
				player.p.x+2, player.p.y+2,
				player.p.x+2, player.p.y-2,
				RED);
			}
	}

	if (player.heading == 3) { 
		graphics->fillTriangle(player.p.x-3, player.p.y+3,
		player.p.x+3, player.p.y,
		player.p.x, player.p.y-3,
		GREEN);
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x-3, player.p.y+3,
				player.p.x+3, player.p.y,
				player.p.x, player.p.y-3,
				RED);
			}
	}

	if (player.heading == 4) { 
		graphics->fillTriangle(player.p.x, player.p.y+4,
		player.p.x+2, player.p.y-2,
		player.p.x-2, player.p.y-2,
		GREEN);
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x, player.p.y+4,
				player.p.x+2, player.p.y-2,
				player.p.x-2, player.p.y-2,
				RED);
			}
	}

	if (player.heading == 5) { 
		graphics->fillTriangle(player.p.x+3, player.p.y+3,
		player.p.x, player.p.y-3,
		player.p.x-3, player.p.y,
		GREEN);
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x+3, player.p.y+3,
				player.p.x, player.p.y-3,
				player.p.x-3, player.p.y,
				RED);
			}
	}

	if (player.heading == 6) { 
		graphics->fillTriangle(player.p.x+4, player.p.y,
		player.p.x-2, player.p.y-2,
		player.p.x-2, player.p.y+2,
		GREEN);
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x+4, player.p.y,
				player.p.x-2, player.p.y-2,
				player.p.x-2, player.p.y+2,
				RED);
			}
	}

	if (player.heading == 7) { 
		graphics->fillTriangle(player.p.x+3, player.p.y-3,
		player.p.x-3, player.p.y,
		player.p.x, player.p.y+3,
		GREEN);
			if (player.loselife == 1) {
				graphics->fillTriangle(player.p.x+3, player.p.y-3,
				player.p.x-3, player.p.y,
				player.p.x, player.p.y+3,
				RED);
			}
	}
}

void drawmissiles(struct missile *list)
{
    while(list) {
        graphics->fillCircle(list->p.x,list->p.y, 2, GREEN);
        list = list->next; 
    }
}

void drawasteroids(struct asteroid *list)
{
    while(list) {
      graphics->fillCircle(list->p.x,list->p.y, 17, YELLOW);
			list = list->next; 
    }
}

void draw(void)
{
    graphics->fillScreen(background);
	
		draw_ship(player);
		drawmissiles(bulletlist);
	  drawasteroids(asteroidslist);
		draw_sidebar(elapsed_time, score, lives);
	
    swap_DBuffer();
}
