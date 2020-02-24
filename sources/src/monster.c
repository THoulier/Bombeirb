#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <sprite.h>
#include <monster.h>





void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
  monster->time=SDL_GetTicks();
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}


struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	monster->direction = NORTH;
	monster->next=NULL;
  return monster;
}

int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

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


struct monster* monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;


int current_time = SDL_GetTicks();
int timer= current_time - monster->time ;
if (timer>2000){
		srand(time(NULL));
		monster->direction = rand()%4;
		switch (monster->direction) {
		case NORTH:
			if (monster_move_aux(monster, map, x, y - 1) && y > 0) {
				monster_set_current_way(monster, NORTH);
        
        monster->y--;

				monster->time=SDL_GetTicks();

				break;
			}
			break;

		case SOUTH:
			if (monster_move_aux(monster, map, x, y + 1) && y < map_get_height(map)-1) {
				monster_set_current_way(monster, 	SOUTH);

				monster->y++;

        monster->time=SDL_GetTicks();

				break;
			}
			break;

		case WEST:
			if (monster_move_aux(monster, map, x - 1, y) && x > 0) {
				monster_set_current_way(monster, WEST);

				monster->x--;

				monster->time=SDL_GetTicks();

				break;
					}
			break;

		case EAST:
			if (monster_move_aux(monster, map, x + 1, y) && x < map_get_width(map)-1) {
				monster_set_current_way(monster,EAST);

				monster->x++;

				monster->time=SDL_GetTicks();

				break;
			}
			break;

		}

}
return monster;
}

struct monster* cell_monster_map(struct monster* monster, struct map* map) {
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {
	    if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
				struct monster* new_one=malloc(sizeof(*new_one));
				new_one->x=i;
				new_one->y=j;
				new_one->direction=NORTH;
				new_one->time=SDL_GetTicks();
				new_one->next=NULL;

				if (monster==NULL)
				monster=new_one;
				else {
					struct monster *tmp =monster;
					while(tmp->next!=NULL)
						tmp=tmp->next;
				tmp->next=new_one;
				monster_set_position(tmp, i, j);}

	    }
	  }
	}
	return monster;
}




void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}
