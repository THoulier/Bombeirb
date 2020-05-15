/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <monster.h>
#include <bomb.h>
#include <bonus.h>
#include <map.h>
#include <save.h>






struct player* player_init(int bombs, short lives) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = SOUTH;
	player->bombs = bombs;
	player->lives=lives;
	player->contact=0;
	player->key=0;
	player->dmg_tmp=0;
	player->bombrange=1;
	player->level=0;
	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

int player_get_key(struct player* player) {
	assert(player);
	return player->key;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

int player_get_bombrange(struct player*player){
	assert(player);
	return player->bombrange;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}
void player_inc_nb_lives(struct player* player) {
	assert(player);
	player->lives += 1;
}
void player_dec_nb_lives(struct player* player) {
	assert(player);
 if(player->lives>0)
	player->lives -= 1;

	}
void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

void player_inc_key(struct player* player) {
	assert(player);
	player->key += 1;
}

void player_dec_key(struct player* player) {
	assert(player);
	player->key -= 1;
}

void player_inc_range(struct player*player){
	assert(player);
	player->bombrange+=1;
}
void player_dec_range(struct player*player){
	assert(player);
	player->bombrange-=1;
}

int player_get_level(struct player*player){
	assert(player);
	return player->level;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
		case CELL_SCENERY:
			if (map_get_compose_type(map,x,y)==CELL_PRINCESS){
				return 1;
			}else{
				return 0;
			}
			break;
		case CELL_BOX:
			return 1;
			break;
		case CELL_BONUS:
			break;
		case CELL_KEY:
			map_set_cell_type(map, x, y,CELL_EMPTY);
			player->key+=1;
			break;
		default:
			break;
	}
	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			if (map_get_cell_type(map,x,y-1)==CELL_BOX
			&& map_get_cell_type(map, x, y-2)==CELL_EMPTY && y-2>=0){
				map_set_cell_type(map,x,y-1,CELL_EMPTY);
				map_set_cell_type(map,x,y-2,CELL_BOX);
			}
			if (player->y >0){
				player->y--;
				move = 1;
			}

			if ((map_get_cell_type(map,x,0)==CELL_BOX && y==1)
			|| (map_get_cell_type(map,x,y-1)==CELL_BOX
			&& map_get_cell_type(map,x,y-2)!=CELL_EMPTY)){
				move=0;
				player->y++;
			}

		}
		break;


	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			if (map_get_cell_type(map,x,y+1)==CELL_BOX  && y+2< map_get_height(map)
			&& map_get_cell_type(map, x, y+2)==CELL_EMPTY){
				map_set_cell_type(map,x,y+1,CELL_EMPTY);
				map_set_cell_type(map,x,y+2,CELL_BOX);
			}
			if (player->y < map_get_height(map)-1){
				player->y++;
				move = 1;
			}
			if ((map_get_cell_type(map,x,map_get_height(map) - 1)==CELL_BOX
			&& y==map_get_height(map) - 2) || (map_get_cell_type(map,x,y+1)==CELL_BOX
			&& map_get_cell_type(map,x,y+2)!=CELL_EMPTY)){
				move=0;
				player->y--;
			}
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			if (map_get_cell_type(map,x-1,y)==CELL_BOX && x-2>=0
			&& map_get_cell_type(map, x-2, y)==CELL_EMPTY){

				map_set_cell_type(map,x-1,y,CELL_EMPTY);
				map_set_cell_type(map,x-2,y,CELL_BOX);
			}
			if (player->x > 0){
				player->x--;
				move = 1;
		}
		if ((map_get_cell_type(map,0,y)==CELL_BOX && x==1)
		|| (map_get_cell_type(map,x-1,y)==CELL_BOX
		&& map_get_cell_type(map,x-2,y)!=CELL_EMPTY)){
			move=0;
			player->x++;
}
}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			if (map_get_cell_type(map,x+1,y)==CELL_BOX
			&& x+2<map_get_width(map) && map_get_cell_type(map, x+2, y)==CELL_EMPTY){
				map_set_cell_type(map,x+1,y,CELL_EMPTY);
				map_set_cell_type(map,x+2,y,CELL_BOX);
			}
			if (player->x < map_get_width(map) - 1){
				player->x++;
				move = 1;
		}
		if ((map_get_cell_type(map,map_get_width(map)-1,y)==CELL_BOX
		&& x==map_get_width(map)-2) || (map_get_cell_type(map,x+1,y)==CELL_BOX
		&& map_get_cell_type(map,x+2,y)!=CELL_EMPTY)){
			move=0;
			player->x--;
		}
	}

		break;

}





	return move;
}

void player_display(struct player* player) {
	assert(player);
	if (player->dmg_tmp > 0) {
		player->dmg_tmp--;
		if (player->dmg_tmp % 2) {
			window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
		}
	}
	else
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

void player_bomb(struct player* player, struct map* map) {

	if (player->bombs!=0){
				bomb_insertion(player->x,player->y,player->bombrange,player->level,3);
				player_dec_nb_bomb(player);
			}
		}

void player_get_bonus(struct player*player, struct map*map){
	int x=player->x;
	int y=player->y;

	switch(map_get_compose_type(map,x,y)&15){
		case BONUS_BOMB_NB_DEC:
			if (player->bombs>0){
				player_dec_nb_bomb(player);
			}	
			break;
		case BONUS_BOMB_NB_INC:
			if (player->bombs<9){
				player_inc_nb_bomb(player);
			}
			break;
		case BONUS_BOMB_RANGE_DEC:
			if (player->bombrange>1){
				player_dec_range(player);
			}
			break;
		case BONUS_BOMB_RANGE_INC:
			if (player->bombrange<9){
				player_inc_range(player);
			}
			break;
		case BONUS_LIFE:
			if (player->lives<9){
				player_inc_nb_lives(player);
			}
			break;
		default:
			break;
	}
	map_set_cell_type(map,x,y,CELL_EMPTY);

}

void player_change_level(struct player*player, int level){
	assert(player);
	player->level=level;
}


void player_save(struct player *player){
	
	save_player(player->x,player->y,player->lives,player->direction,player->level,player->bombrange,player->key,player->bombs,player->contact,player->dmg_tmp);

}