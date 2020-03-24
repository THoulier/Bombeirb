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




struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
//	game->maps[0] = get_map ("map/map_1");
	game->maps[0] = map_get_static();


	game->levels = 1;
	//struct listbomb *list=NULL;
//	listbomb_init();

	game->level = 0;
	game->monster=NULL;

	game->player = player_init(5,6);
	game->monster=cell_monster_map(game->monster, game_get_current_map(game));
	// Set default location of the player
	player_set_position(game->player, 1, 0);
	//map_bonus_init(game->maps[game->level]);

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

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 8 * SIZE_BLOC) / 6;
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
}

void game_display(struct game* game) {
	assert(game);
	struct map* map = game_get_current_map(game);
	struct player* player = game_get_player(game);


	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));

	player_display(game->player);

	struct monster*tmp=game->monster;
	while (tmp!=NULL){
		if(map_get_cell_type(game_get_current_map(game),tmp->x,tmp->y)==CELL_EXPLOSION)
			game->monster=kill_monster(game->monster,tmp);
		monster_move(tmp,game_get_current_map(game));
		monster_display(tmp);


		tmp=tmp->next;
	}
	tmp=game->monster;
	while (tmp!=NULL){
		int current_time = SDL_GetTicks();
		if(game->player->x == tmp->x && game->player->y==tmp->y ){

			if ((current_time - game->player->contact) > 1000){
					player_dec_nb_lives(game->player);
						game->player->dmg_tmp=29;
					 game->player->contact= SDL_GetTicks();}}

		tmp=tmp->next;}
		free(tmp);
		int current_time = SDL_GetTicks();
		if(map_get_cell_type(map,player->x,player->y)==CELL_EXPLOSION && player->lives>0){
			if ((current_time - game->player->contact) > 1000){
		player_dec_nb_lives(player);
		game->player->dmg_tmp=29;
	 	game->player->contact= SDL_GetTicks();}}


	listbomb_refresh(player,map);
	if(map_get_cell_type(map,player->x,player->y)==CELL_BONUS) {
		player_get_bonus(player,map);
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

			  	player_bomb(player, map);


				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game

	return 0;
}
