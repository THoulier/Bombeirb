#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <constant.h>



struct bomb{
	int x, y, time ,clic;
	enum etats etat;
 struct bomb * next;
struct bomb* prev;
};


void bomb_display(struct bomb* bomb);
struct bomb* bomb_init();
struct bomb* start_bomb(struct bomb *bomb, struct map * map );
struct bomb *chaine_init(struct bomb * bomb, int bombs );
