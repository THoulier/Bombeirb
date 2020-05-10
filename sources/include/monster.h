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
	//struct monster* next;
	//struct monster* prev;
};
struct listmonster{
  struct monster *monster;
  struct listmonster *next;
};
void listmonster_refresh(struct game*game,struct player*player);

void listmonster_init(struct map*map,int nummap);

void monster_append(int x, int y, int nummap);


void monster_set_position(struct monster *monster, int x, int y);
void monster_set_current_way(struct monster* monster, enum direction way);
struct monster* monster_init(int x,int y, int nummap);
int monster_move_aux(struct monster* monster, struct map* map, int x, int y);
struct monster* monster_move(struct monster* monster, struct map* map,int time_level);
void monster_display(struct game * game,struct monster* monster);
struct monster* cell_monster_map(struct monster* monster, struct map* map);
//struct monster* kill_monster(struct monster* monster,struct monster* tmp);
void monster_kill(struct monster *monster);
void listmonster_save();
int listmonster_get_length(struct listmonster * listmonster);
#endif
