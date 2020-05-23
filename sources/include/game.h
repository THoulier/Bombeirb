/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

// Abstract data type
struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster* monster;
	struct bomb* bomb;
	struct listbomb *listbomb;
};

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

//the player open a door
void game_door(struct game* game);
void game_change_map(struct game* game,int newMap, int x, int y);

//the game is on pause
int game_pause(struct game *game );

//the game is end
int game_end(struct game *game );

//get the map
struct map* game_get_nummap(struct game* game,int nummap);
int game_get_level(struct game* game);

//load a saved game
struct game* game_load(void);

#endif /* GAME_H_ */
