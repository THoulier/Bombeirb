#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <constant.h>

struct monster{
	int x, y, time;
	enum direction direction;
	struct monster* next;
};




void monster_set_position(struct monster *monster, int x, int y);
void monster_set_current_way(struct monster* monster, enum direction way);
struct monster* monster_init();
int monster_move_aux(struct monster* monster, struct map* map, int x, int y);
struct monster* monster_move(struct monster* monster, struct map* map);
void monster_display(struct monster* monster);
struct monster* cell_monster_map(struct monster* monster, struct map* map);
