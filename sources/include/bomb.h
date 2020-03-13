#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <constant.h>



struct bomb{
	int x, y, time ;
	enum etats etat;
 	struct bomb * next;
};

struct listbomb {
  struct bomb * first;
  struct listbomb * next;
};

struct bomb * bomb_init(int x, int y);
void listbomb_init();
void bomb_insertion(int x, int y);
void bomb_sup(struct bomb*bomb);
void listbomb_refresh( struct player *player);
int bomb_start(struct bomb *bomb);
void bomb_display(struct bomb*bomb);
void explo_display(struct bomb*bomb);


