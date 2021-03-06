/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player{
	int x, y;
	short lives;
  	int contact ;
	enum direction direction;
	int bombs;
	int key;
	int dmg_tmp;
	int bombrange;
	int level;
};

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number, short lives);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the player parameters
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_inc_key(struct player* player);
void player_dec_key(struct player* player);
void player_inc_range(struct player*player);
void player_dec_range(struct player*player);
void player_inc_nb_lives(struct player* player);
void player_dec_nb_lives(struct player* player);

//get the player parameters
void player_get_bonus(struct player*player, struct map*map);
int player_get_bombrange(struct player*player);
int player_get_level(struct player*player);
int player_get_key(struct player* player);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

//a player put a bomb on the map
void player_bomb(struct player* player, struct map* map);

//a player open a door
void player_change_level(struct player*player, int level);	

//save the player parameters
void player_save(struct player*player);
#endif /* PLAYER_H_ */
