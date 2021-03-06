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

void listmonster_null(){
  first=NULL;
}


void listmonster_init(struct map*map,int nummap){
	/*initialisation of the monster on the maps*/
  	int i=0;
  	int j=0;

  	for(i=0;i<map_get_width(map);i++){
   		for(j=0;j<map_get_height(map);j++){
      		if(map_get_cell_type(map,i,j)==CELL_MONSTER){
          		monster_append(i,j,nummap,SOUTH);
      		}
    	}
  	}
}

void monster_append(int x, int y, int nummap,enum direction direction){
	/*add a monster in the list*/
    struct monster * monster = monster_init(x,y,nummap,direction);
	if (first==NULL){
		first=malloc(sizeof(*first));
		first->monster=monster;
		first->next=NULL;
    }
	else{
	struct listmonster * listmonster=malloc(sizeof(*listmonster));
    listmonster->monster=monster;
    listmonster->next=first;
    first=listmonster;
	}
}


void listmonster_refresh(struct game*game,struct player*player){
	/*Refresh the action linked to a monster in game: moves, contact with the player, explosion*/
  	struct listmonster*listmonster=first;
  	int nummap=game_get_level(game);

  	srand(time(NULL));
	int random = rand()%4;

  	while(listmonster!=NULL){
		monster_display(game,listmonster->monster);
		monster_move(listmonster->monster,game_get_nummap(game,listmonster->monster->nummap),2000-nummap*200,random); //the monster's speed increases with the level
		
		int current_time=SDL_GetTicks();

		/*Manage the contact between a monster and the player*/
		if(game->player->x == listmonster->monster->x && game->player->y==listmonster->monster->y){
			if(listmonster->monster->nummap==game_get_level(game)){
				if ((current_time - game->player->contact) > 1000){
					player_dec_nb_lives(game->player);
					game->player->dmg_tmp=29;
					game->player->contact= SDL_GetTicks();
				}
			}	
		}
		/*kill a monster if it is in a explosion*/
		if(map_get_cell_type(game_get_nummap(game,listmonster->monster->nummap),listmonster->monster->x,listmonster->monster->y)==CELL_EXPLOSION){
			monster_kill(listmonster->monster);
		}
		random = rand()%4; //new initialization of the random number 
		listmonster=listmonster->next;
	}
}


void monster_kill(struct monster *monster){
	/*Drop the monster from the list*/
	struct listmonster * prev;
	struct listmonster * next;
	struct listmonster  * listmonster = first;
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


void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}


struct monster* monster_init(int x, int y, int nummap,enum direction direction) {
	/*initialisation of the parameters for a monster*/
	struct monster* monster = malloc(sizeof(*monster));
	monster->direction = direction;
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


struct monster* monster_move(struct monster* monster, struct map* map,int time_level, int random) {
	/*manage the moves of a monster*/
	int x = monster->x;
	int y = monster->y;
	int current_time = SDL_GetTicks();
	int timer= current_time - monster->time ;
	if (timer > time_level){
		monster->direction = random;	 //random direction
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

void monster_display(struct game*game,struct monster* monster) {
	/*display the monsters which are in the same level as the player*/
	if(monster){
		if(monster->nummap==game_get_level(game)){
			window_display_image(sprite_get_monster(monster->direction),monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
		}
	}
}

int listmonster_get_length(struct listmonster * listmonster){
	/*to get the length of the list*/
	int nummonster=0;
	listmonster=first;

	while(listmonster){
		nummonster +=1;
		listmonster=listmonster->next;
	}

	return nummonster;
}


void listmonster_save(){
	/*save the monster list parameters*/
	struct listmonster *listmonster=first;
	save_nummonster(listmonster_get_length(listmonster));

	while(listmonster){
		save_monster(listmonster->monster->x,listmonster->monster->y,listmonster->monster->direction,listmonster->monster->nummap);
		listmonster=listmonster->next;
	}

}



