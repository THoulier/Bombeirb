/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <monster.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <player.h>
#include <bomb.h>
#include <constant.h>
#include <bonus.h>
#include <save.h>


 



struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->levels = 7;

	for (int nummap=0; nummap<=game->levels; nummap++){
		game->maps[nummap] = get_map(nummap);
	}
	
	game->level = 0;
	game->player = player_init(1,3);
	player_set_position(game->player, 1, 0);

	return game;
}

struct game* game_load(void) {
	sprite_load(); // load sprites into process memory
	struct game* game = malloc(sizeof(*game));
	struct player* player = malloc(sizeof(*player));

	game->maps = malloc(sizeof(struct game));
	game->levels = 7;

	for (int nummap=0; nummap<=game->levels; nummap++){
		game->maps[nummap] = get_map_saved(nummap);
	}

	game->player=player;
	load_player(game->player);
	game->level=game->player->level;
	load_listbomb();
	load_listmonster();

	window_resize(SIZE_BLOC * map_get_width(game_get_current_map(game)),SIZE_BLOC * map_get_height(game_get_current_map(game)) + LINE_HEIGHT + BANNER_HEIGHT);

	return game;
}

void game_free(struct game* game) {
	assert(game);
	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}

struct monster* game_get_monster(struct game* game) {
	assert(game);
	return game->monster;
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_banner_display(struct game* game) {
	assert(game);
	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 10 * SIZE_BLOC) / 6;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(game_get_player(game)->lives), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(game_get_player(game)->bombrange), x, y);

	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_key(game->player)), x, y);

	x = 5 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_banner_flag(), x, y);

	x = 5 * white_bloc + 9 * SIZE_BLOC;
	window_display_image(sprite_get_number((game->level)+1), x, y);
}

void game_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);
	struct player* player = game_get_player(game);

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);

	int current_time = SDL_GetTicks();
	if(map_get_cell_type(map,player->x,player->y)==CELL_EXPLOSION && player->lives>0){
		if ((current_time - game->player->contact) > 1000){
			player_dec_nb_lives(player);
			game->player->dmg_tmp=29;
	 	}
		game->player->contact= SDL_GetTicks();
	}

	listmonster_refresh(game,player);
	listbomb_refresh(player,map,game);

	if(map_get_cell_type(map,player->x,player->y)==CELL_BONUS) {
		player_get_bonus(player,map);
	}

	if (game->player->lives<1){ 
		window_display_image(sprite_get_gameover(),((map_get_width(map)*SIZE_BLOC)-480)/2,((map_get_height(map)*SIZE_BLOC)-480)/2);
		window_display_image(sprite_get_press_esc(),150,(map_get_height(map)*SIZE_BLOC)-30);
	}

	if (map_get_compose_type(map,player->x,player->y)==CELL_PRINCESS){
		window_display_image(sprite_get_youwin(),((map_get_width(map)*SIZE_BLOC)-480)/2,((map_get_height(map)*SIZE_BLOC)-480)/2);
		window_display_image(sprite_get_press_esc(),150,(map_get_height(map)*SIZE_BLOC)-30);
	}

	window_refresh();
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:

				return 1;
			case SDLK_o:
				game_door(game);
			break;
			case SDLK_s:
				listmonster_save();
				listbomb_save();
				player_save(game->player);
				for (int nummap=0; nummap<=game->levels; nummap++){
					map_save(game->maps[nummap],nummap);
				}
			break;
			case SDLK_p: 
				return (game_pause(game));
				break;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				if (map_get_cell_type(map,player->x,player->y)!=CELL_DOOR){
			  		player_bomb(player, map);
				}
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}




void game_change_map(struct game* game,int nummap, int x, int y) { 
	game->level = nummap;
	player_set_position(game->player, x, y);
	player_change_level(game->player,nummap);
	window_resize(SIZE_BLOC * map_get_width(game_get_current_map(game)),SIZE_BLOC * map_get_height(game_get_current_map(game)) + LINE_HEIGHT + BANNER_HEIGHT);
}

void game_door(struct game* game) {
	assert(game);
	int type=map_get_compose_type(game->maps[game->level],player_get_x(game->player),player_get_y(game->player));
	if ((type & 0xf0)==CELL_DOOR){
		int x=0;
		int y=0;
		int nummap=(type>>1) & 0x07;
		if (nummap > game->level){
			switch (nummap){
				case 1:
					x=1;
					y=1;
				break;
				case 2:
					x=2;
					y=10;
				break;
				case 3:
					x=0;
					y=4;
				break;
				case 4:
					x=3;
					y=13;
				break;
				case 5:
					x=3;
					y=2;
				break;
				case 6:
					x=13;
					y=6;
				break;
				case 7:
					x=5;
					y=15;
				break;
			}
		}
		if (nummap < game->level){
			switch (nummap){
				case 0:
					x=2;
					y=11;
				break;
				case 1:
					x=11;
					y=11;
				break;
				case 2:
					x=12;
					y=2;
				break;
				case 3:
					x=9;
					y=13;
				break;
				case 4:
					x=4;
					y=5;
				break;
				case 5:
					x=4;
					y=5;
				break;
				case 6:
					x=0;
					y=6;
				break;

			}
		}
		if ((type & 0x01)){
			game_change_map(game, nummap, x, y);
		}
		else {
			if (player_get_key(game->player)){
				player_dec_key(game->player);
				map_set_cell_type(game->maps[game->level], player_get_x(game->player), player_get_y(game->player),type | 0x01 );
				game_change_map(game,nummap,x,y);
			}
		}
	}
}


int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game
	if (game->player->lives<1){
		return game_end(game);
	}
	if (map_get_compose_type(game->maps[game->level],game->player->x,game->player->y)==CELL_PRINCESS){
		return game_end(game);
	}
	return 0;
}

int game_pause(struct game *game ){
	int pause=1;
	int time= SDL_GetTicks();

	SDL_Event event;
	while (pause){
		SDL_WaitEvent(&event);
		switch (event.type){
			case SDL_QUIT:
				return 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_p:
						listbomb_pause(SDL_GetTicks()-time);
						pause=0;
						return 0;
						break;
					case SDLK_ESCAPE:
						return 1;
						break;
					default:
						break;
				}
			break;
			default:
			break;
		}
	}
	return 0;
}




int game_end(struct game *game ){
	SDL_Event event;
	SDL_WaitEvent(&event);
		switch (event.type){
			case SDL_QUIT:
				return 1;
				break;

			case SDL_KEYDOWN:
				if ( event.key.keysym.sym == SDLK_ESCAPE ){
						return 1;
				}
				break;
			default:
				break;
		}
	return 0;
}

struct map* game_get_nummap(struct game* game,int nummap){
	assert(game);
	return game->maps[nummap];
}


int game_get_level(struct game* game) {
	assert(game);
	return game->level;
}


