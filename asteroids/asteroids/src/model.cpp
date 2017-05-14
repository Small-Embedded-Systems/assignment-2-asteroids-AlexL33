/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "utils.h"
#include "asteroids.h"

// Create bounds
enum {
LEFTBOUND = 25,
RIGHTBOUND = 400,
UPPERBOUND = 50,
LOWERBOUND = 300,
};


//Initialise list
const size_t MAXSize = 10;
miss_t mdata[MAXSize];
miss_t *mfreenodes;
void minitialise(void) {
    int c;
    for(c=0 ; c<(MAXSize-1) ; c++){
        mdata[c].next = &mdata[c+1];
    }
    mdata[c].next = NULL;
		mfreenodes = &mdata[0];
} 

const size_t AMAXSize = 10;
ast_t adata[AMAXSize];
ast_t *afreenodes;
void ainitialise(void) {
    int c;
    for(c=0 ; c<(AMAXSize-1) ; c++){
        adata[c].next = &adata[c+1];
    }
    adata[c].next = NULL;
		afreenodes = &adata[0];
} 

//Allocate nodes
miss_t *mallocnode(void) {
	miss_t *mnode = NULL;
	if (mfreenodes) {
		mnode = mfreenodes;
		mfreenodes = mfreenodes->next;
	}
	return mnode;
}

ast_t *aallocnode(void) {
	ast_t *anode = NULL;
	if (mfreenodes) {
		anode = afreenodes;
		afreenodes = afreenodes->next;
	}
	return anode;
}

//Release nodes
void mfreenode(miss_t *mn) {
	mn->next = mfreenodes;
	mfreenodes = mn;
}

void afreenode(ast_t *an) {
	an->next = afreenodes;
	afreenodes = an;
}

/* Creates the missile and tracks it to the ships 
current location and heading*/
void mcreate(struct missile *mr) {
	if (mr->hit == 0) {
		 mr->p.x = player.p.x;
		 mr->p.y = player.p.y;

		if (player.heading == 0) { // 
			mr->v.y = -1; mr->v.x = 0;
		}
		else if (player.heading == 2) { 
			mr->v.y = 0; mr->v.x = -1;
		}
		else if (player.heading == 4) { 
			mr->v.y = 1; mr->v.x = 0;
		}
		else if (player.heading == 6) { 
			mr->v.y = 0; mr->v.x = 1;
		}
		else if (player.heading == 1) { 
			mr->v.y = -1; mr->v.x = -1;
		}
		else if (player.heading == 3) { 
			mr->v.y = 1; mr->v.x = -1;
		}
		else if (player.heading == 5) { 
			mr->v.y = 1; mr->v.x = 1;
		}
		else if (player.heading == 7) { 
			mr->v.y = -1; mr->v.x = 1;
		}
			mr->ttl = 10; 
			mr->hit = 0;
	}
	else if (mr->hit == 1) {
		mr->p.x = 500;
		mr->p.y = 500;
		mr->v.y = 0;
		mr->v.x = 0;
		mr->ttl = 5;
		mr->hit = 0;
	}
}

/*Create asteroid within bounds*/
void acreate(struct asteroid *ar) {
	  ar->p.x = randrange(RIGHTBOUND,LEFTBOUND);
    ar->p.y = randrange(UPPERBOUND,LOWERBOUND);
			while (((ar->p.y > UPPERBOUND) && (ar->p.y < LOWERBOUND)) && ((ar->p.x > LEFTBOUND) && (ar->p.x < RIGHTBOUND))) {
					  ar->p.x = randrange(UPPERBOUND,RIGHTBOUND);
						ar->p.y = randrange(UPPERBOUND,LOWERBOUND);
		}
		ar->v.x = randrange(-1,2);
		ar->v.y = randrange(-1,2);
		
		ar->ttl = 20;
		ar->hit = 0;
}



// Update
const float Df = 0.01;
void mupdate(struct missile *ml)
{
		// For each missile
    for( ; ml ; ml = ml->next ) {
        ml->p.x += ml->v.x; 
        ml->p.y += ml->v.y;
				ml->ttl -= Df;
        if( (ml->next->ttl<=0) || (ml->hit == 1) ) {
            struct missile *mexpired = ml->next;
            ml->next = ml->next->next;
            mfreenode(mexpired);
        }
    }
}

void aupdate(struct asteroid *al)
{
		//For each asteroid
    for( ; al ; al = al->next ) {
        al->p.x += al->v.x; 
        al->p.y += al->v.y;
			
				// Wrapper to ensure asteroids will appear from the 
				//other side of screen when out of bounds
				if (al->p.x < LEFTBOUND) {
					al->p.x = RIGHTBOUND;
				}
				if (al->p.x > RIGHTBOUND) {
					al->p.x = LEFTBOUND;
				}
				if (al->p.y < UPPERBOUND) {
					al->p.y = LOWERBOUND;
				}
				if (al->p.y > LOWERBOUND) {
					al->p.y = UPPERBOUND;
				}
				al->ttl -= Df;
				
				//Remove if destroyed 
				if(   ((al->next->ttl<=0) && ((al->next->p.x > RIGHTBOUND-5) || (al->next->p.x < LEFTBOUND+5)
					|| (al->next->p.y > LOWERBOUND-5) || (al->next->p.y < UPPERBOUND+5))) || (al->hit == 1)) {
					struct asteroid *aexpired = al->next;
					al->next = al->next->next;
					afreenode(aexpired);
				}
    }
	
}

//Manage objects
struct missile *bulletlist = NULL;
void missile_handler(void) {
	struct missile *missignite = mallocnode();
        if(missignite) {
            missignite->next = bulletlist;
            bulletlist = missignite;
            mcreate(missignite);
        }

        mupdate(bulletlist);
}

int ct = 500;
struct asteroid *asteroidslist = NULL;

/* Create an asteroid every 3 seconds */
void asteroid_system(void) {
	ct--;
	if (ct == 0) {
		struct asteroid *aignite = aallocnode();
					if(aignite) {
							aignite->next = asteroidslist;
							asteroidslist = aignite;
							acreate(aignite);
						ct = 300;
        }
			}
}

// Reset lists
void reset() {
	asteroidslist = NULL;
	bulletlist = NULL;
}

//Collision detection
void missiles_hit_rocks(struct missile *miss, struct asteroid *ass) {
	
	for ( ; miss ; miss = miss->next ) {
		for ( ; ass ; ass = ass->next ) {
			if (((miss->p.x > ass->p.x-20) && (miss->p.x < ass->p.x+20)) && 
				 ((miss->p.y > ass->p.y-20) && (miss->p.y < ass->p.y+20))) {
					 ass->hit = 1;
					 miss->hit = 1;
					 acreate(ass);
					 mcreate(miss);
		  }
	  }	
	}
}

/* If collison with an asteroid occurs return true
	 else false is returned */
bool ship_hits(struct asteroid *ass) {
	for( ; ass ; ass = ass->next ) {
		if (((player.p.x > ass->p.x-20) && (player.p.x < ass->p.x+20)) && 
				((player.p.y > ass->p.y-20) && (player.p.y < ass->p.y+20))) {
					return true;
				}
		}
	return false;
}


//Ship movement
ship move_ship(ship player, float Dt) {
		player.p.y += player.v.y;
		player.p.x += player.v.x;
	
	if (player.p.x < 1) {
		player.p.x = 473;
	}
	if (player.p.x > 474) {
		player.p.x = 1;
	}
	if (player.p.y < 0) {
		player.p.y = 250;
	}
	if (player.p.y > 270) {
		player.p.y = 1;
	}
	
	return player;
}


//Game Physics
int hit = 0;
int hct = 100;
void physics(void)
{
	if (!paused) {
    player = move_ship(player, Dt);
	  asteroid_system();

		if (ship_hits(asteroidslist)) {
			if (hit == 0) {
				lives--;
				hit = 1;
			}
	}
		if (hit == 1) {
			player.loselife = 1;
				hct--;
				if (hct == 0) {
					player.loselife = 0;
					hit = 0;
					hct = 100;
			  }
		}
	  mupdate(bulletlist);
		aupdate(asteroidslist);
		missiles_hit_rocks(bulletlist, asteroidslist);
	}
}

