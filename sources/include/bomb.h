#ifndef M_M_
#define M_M_
#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <constant.h>
#include <game.h>



struct bomb{
	int x, y, time , nummap, range;
	enum etats etat;
};

struct listbomb {
  struct bomb * bomb;
  struct listbomb * next;
};

struct bomb * bomb_init(int x, int y, int range, int nummap, int etat);
void listbomb_init();
void bomb_insertion(int x, int y, int range, int nummap,int etat);
void listbomb_refresh(struct player * player , struct map* map, struct game * game);
int bomb_start(struct bomb *bomb,struct map* map,struct player*player, int etat);
void bomb_display(struct bomb*bomb, struct game *game);
void explo_display(struct bomb*bomb,struct player *player,struct map* map);
#endif
void listbomb_pause(int time_pause);
void listbomb_save();
int listbomb_get_length(struct listbomb * listbomb);
void bomb_sup(struct bomb*bomb);
void explo_end(struct map* map,int x,int y);
