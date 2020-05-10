#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <sprite.h>
#include <monster.h>
#include <game.h>
#include <save.h>





struct listmonster *first=NULL;

void listmonster_init(struct map*map,int nummap){
  int i=0;
  int j=0;

  for(i=0;i<map_get_width(map);i++){
    for(j=0;j<map_get_height(map);j++){
      if(map_get_cell_type(map,i,j)==CELL_MONSTER){
          monster_append(i,j,nummap);
      }
    }
  }
}

void monster_append(int x, int y, int nummap){
    struct monster * monster = monster_init(x,y,nummap);
	if (first==NULL){
    first=malloc(sizeof(*first));
    first->monster=monster;
    first->next=NULL;
    }
	struct listmonster * listmonster=malloc(sizeof(*listmonster));
    listmonster->monster=monster;
    listmonster->next=first;
    first=listmonster;
}


void listmonster_refresh(struct game*game,struct player*player){
  	struct listmonster*listmonster=first;
  	int nummap=game_get_level(game);
  	while(listmonster!=NULL){
    monster_display(game,listmonster->monster);
	monster_move(listmonster->monster,game_get_nummap(game,listmonster->monster->nummap),2000-nummap*400);
  	int current_time=SDL_GetTicks();

	if(game->player->x == listmonster->monster->x && game->player->y==listmonster->monster->y){
		if(listmonster->monster->nummap==game_get_level(game)){
			if ((current_time - game->player->contact) > 1000){
				player_dec_nb_lives(game->player);
				game->player->dmg_tmp=29;
				game->player->contact= SDL_GetTicks();
			}
		}	
	}

	if(map_get_cell_type(game_get_nummap(game,listmonster->monster->nummap),listmonster->monster->x,listmonster->monster->y)==CELL_EXPLOSION){
		monster_kill(listmonster->monster);
	}

    listmonster=listmonster->next;
  }
}


void monster_kill(struct monster *monster){
  struct listmonster *prev;
  struct listmonster *next;
  struct listmonster  *listmonster = first;
  if (listmonster->monster==monster){
    first=listmonster->next;
  }
  else{
    while (listmonster!=NULL){
      	if (listmonster->next->monster==monster){
			prev=listmonster;
			listmonster=listmonster->next;
			next=listmonster->next;
			listmonster=NULL;
		}
		else {
			listmonster=listmonster->next;
		}
    }
    prev->next=next;
  }
  free(monster);
}
/*
struct monster* kill_monster(struct monster* monster,struct monster* tmp){
	if(monster==NULL)
		return NULL;
	else if(tmp->prev==NULL){
		return monster->next;
	}
	else if (tmp->next==NULL){
		tmp->prev->next=NULL;
	}
	else{
		struct monster *tmpp=monster;
		while(tmpp!=tmp)
			tmpp=tmpp->next;
		tmpp->prev->next=tmpp->next;
		free(tmp);
		return tmpp;
	}
	return 0;
}

*/

void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}


struct monster* monster_init(int x, int y, int nummap) {
	struct monster* monster = malloc(sizeof(*monster));
	monster->direction = NORTH;
	monster->time=SDL_GetTicks();
  	monster->x=x;
  	monster->y=y;
	monster->nummap=nummap;
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
			return 0;
		break;
    	case CELL_DOOR:
 			return 0;
		break;
		case CELL_KEY:
			return 0;
		break;
		case CELL_MONSTER:
			return 0;
		break;

		default:

		break;
	}

	return 1;
}


struct monster* monster_move(struct monster* monster, struct map* map,int time_level) {
	int x = monster->x;
	int y = monster->y;
	int current_time = SDL_GetTicks();
	int timer= current_time - monster->time ;
	if (timer > time_level){

		monster->direction = rand()%4;
		switch (monster->direction) {
			case NORTH:
				if (monster_move_aux(monster, map, x, y - 1) && y > 0) {
        			map_set_cell_type(map,x,y,CELL_EMPTY);
					map_set_cell_type(map,x,y-1,CELL_MONSTER);
        			monster->y--;
					monster->time=current_time;
					break;
				}
			break;

			case SOUTH:
				if (monster_move_aux(monster, map, x, y + 1) && y < map_get_height(map)-1) {
					map_set_cell_type(map,x,y,CELL_EMPTY);
					map_set_cell_type(map,x,y+1,CELL_MONSTER);
					monster->y++;
        			monster->time=current_time;
					break;
				}
			break;

			case WEST:
				if (monster_move_aux(monster, map, x - 1, y) && x > 0) {
					map_set_cell_type(map,x,y,CELL_EMPTY);
					map_set_cell_type(map,x-1,y,CELL_MONSTER);
					monster->x--;
					monster->time=current_time;
					break;
				}
			break;

			case EAST:
				if (monster_move_aux(monster, map, x + 1, y) && x < map_get_width(map)-1) {
					map_set_cell_type(map,x,y,CELL_EMPTY);
					map_set_cell_type(map,x+1,y,CELL_MONSTER);
					monster->x++;
					monster->time=current_time;
					break;
				}
			break;
		}
	}
	return monster;
}

/*
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
				new_one->prev=NULL;

				if (monster==NULL)
					monster=new_one;

				else {
					struct monster *tmp =monster;
					while(tmp->next!=NULL){
						tmp=tmp->next;
					}
					new_one->prev=tmp;
					tmp->next=new_one;
				}
			}
	  	}	
	}
	return monster;
}

struct monster* monster_append(struct monster * listmonster,struct monster* monster){
	if(listmonster==NULL)
		listmonster=monster;

	else {
		struct monster* tmp=listmonster;
		while(tmp->next!=NULL)
			tmp=tmp->next;
		monster->prev=tmp;
		tmp->next=monster;
	}
	return listmonster;
}
*/
/*

void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}*/
void monster_display(struct game*game,struct monster* monster) {
	if(monster){
		if(monster->nummap==game_get_level(game)){
			window_display_image(sprite_get_monster(monster->direction),monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
		}
	}
}

int listmonster_get_length(struct listmonster * listmonster){
	int nummonster=0;
	listmonster=first;
	while(listmonster->next != NULL){
		nummonster +=1;
		listmonster=listmonster->next;
	}
	return nummonster;
}


void listmonster_save(){
	struct listmonster *listmonster=first;
	save_nummonster(listmonster_get_length(listmonster));

	while(listmonster->next != NULL){
		save_monster(listmonster->monster->x,listmonster->monster->y,listmonster->monster->direction,listmonster->monster->nummap);
		listmonster=listmonster->next;
	}

}


