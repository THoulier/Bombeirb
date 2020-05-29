#ifndef MO_MO
#define MO_MO
#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <game.h>


struct monster{
	int x, y, time, nummap;
	enum direction direction;
};

struct listmonster{
  struct monster *monster;
  struct listmonster *next;
};

//functions to manage the list
void listmonster_refresh(struct game*game,struct player*player);
void listmonster_init(struct map*map,int nummap);
void monster_append(int x, int y, int nummap,enum direction direction);
void listmonster_save();
int listmonster_get_length(struct listmonster * listmonster);
void listmonster_null();
void monster_kill(struct monster *monster);


//functions to manage a monster
void monster_set_position(struct monster *monster, int x, int y);
void monster_set_current_way(struct monster* monster, enum direction way);
struct monster* monster_init(int x, int y, int nummap,enum direction direction);
int monster_move_aux(struct monster* monster, struct map* map, int x, int y);
struct monster* monster_move(struct monster* monster, struct map* map,int time_level, int random);
void monster_display(struct game * game,struct monster* monster);




#endif
