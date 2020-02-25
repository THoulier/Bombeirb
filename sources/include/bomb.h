#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <constant.h>



struct bomb{
	int x, y, time;
	enum etats etat;
	
};


void bomb_display(struct bomb* bomb);
struct bomb* bomb_init();
struct bomb* start_bomb(struct bomb* bomb, struct map * map , int x , int y );
