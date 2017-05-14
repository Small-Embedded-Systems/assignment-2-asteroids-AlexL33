/* Game state */

extern float elapsed_time; /* game timer for all lives */
extern int   score;        /* score count for current lives */
extern int 	 lastscore;		 
extern int   lives;        
extern bool paused;				 

extern struct ship player;

extern struct asteroid *asteroidslist; /* array of rocks / pointer to linked-list */
extern struct missile *bulletlist;  /* array of bullets / pointer to linked-list */

extern const float Dt; 
