#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>

struct monster{
	int x, y, time;
	enum direction direction;


};

void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
  monster->time=0;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}


struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->direction = SOUTH;
  return monster;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_BOX:
		return 0;
		break;

	case CELL_BONUS:
		break;

	default:

		break;
	}

	// Player has moved
	return 1;
}

void monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
int current_time = SDL_GetTicks();
int timer= current_time - monster->time ;
if (timer>2000){
		monster->direction = rand()%4;
		switch (monster->direction) {
		case NORTH:
			if (monster_move_aux(monster, map, x, y - 1) && y > 0) {
				monster_set_current_way(monster, NORTH);
        map_set_cell_type(map, x, y , CELL_EMPTY);
        monster->y--;
        move = 1;
				monster->time=SDL_GetTicks();
				break;
			}
			break;

		case SOUTH:
			if (monster_move_aux(monster, map, x, y + 1) && y < map_get_height(map)-1) {
				monster_set_current_way(monster, 	SOUTH);
				map_set_cell_type(map, x, y , CELL_EMPTY);
				monster->y++;
        move=1;
        monster->time=SDL_GetTicks();
				break;
			}
			break;

		case WEST:
			if (monster_move_aux(monster, map, x - 1, y) && x > 0) {
				monster_set_current_way(monster, WEST);
				map_set_cell_type(map, x, y , CELL_EMPTY)	;
				monster->x--;
        move = 1;
				monster->time=SDL_GetTicks();
				break;
					}
			break;

		case EAST:
			if (monster_move_aux(monster, map, x + 1, y) && x < map_get_width(map)-1) {
				monster_set_current_way(monster,EAST);
				map_set_cell_type(map, x, y , CELL_EMPTY);
				monster->x++;
        move = 1;
				monster->time=SDL_GetTicks();
				break;
			}
			break;

		}
//if (move)
    //	map_set_cell_type(map, x, y , CELL_MONSTER);
}


}
void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}
