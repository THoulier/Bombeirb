/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <monster.h>
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>
#include <bonus.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)



struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{

	assert(map);
	/*if((x<map->width)&(x>=0)&(y<map->height)&(y>=0)){
		return 1;
	}
	else{
		return 0;
	}*/
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum compose_type map_get_compose_type(struct map* map,int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_compose_cell_type(struct map* map, int x, int y, enum compose_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x,y);
		break;
	case BONUS_MONSTER:
		window_display_image(sprite_get_monster(SOUTH), x,y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
		break;

	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);
	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
			case CELL_SCENERY:
		  	display_scenery(map, x, y, type);
		  break;
			case CELL_BOX:
				window_display_image(sprite_get_box(), x, y);
			break;
	    case CELL_BONUS:
	        display_bonus(map, x, y, type);
	        break;
	    case CELL_KEY:
	        window_display_image(sprite_get_banner_key(), x, y);
	        break;
	    case CELL_DOOR:
			if (type & 0x01){
		      	window_display_image(sprite_get_door_opened(), x, y);
			}
			else {
				window_display_image(sprite_get_door_closed(), x, y);
			}
	        break;

		case CELL_EXPLOSION:
          	window_display_image(sprite_get_explo(),x, y);



			break;


			}

	  }
	}
}

/*
struct map* map_get_static(void)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_KEY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_MONSTER, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_MONSTER, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, 50, CELL_KEY, CELL_KEY, CELL_KEY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];
	listmonster_init(map,0);

	return map;
}*/


struct map* get_map (int nummap){
	char *mapp=map_init(nummap);
 	FILE* ptr= fopen(mapp, "r");
 	int width;
 	int height;
 	fscanf(ptr,"%d:%d", &width,&height);
 	struct map * map = map_new(width,height);
	int i, j;
	int valeur;
 	for (i = 0; i < map_get_height(map); i++) {
		for (j = 0; j < map_get_width(map); j++) {
		fscanf(ptr,"%d", &valeur);
		map->grid[j+i*width]=valeur;
	}}
	listmonster_init(map,nummap);

	return map;
}


char * map_init(int nummap) {
    char *path;
      switch (nummap)
      {
		case 0:
			path="map/map_0"; 
		break;
        case 1: 
			path="map/map_1"; 
		break;
        case 2: 
			path="map/map_2"; 
		break;
        case 3: 
			path="map/map_3"; 
		break;
        case 4: 
			path="map/map_4"; 
		break;
      }
	return path;
   }

void map_save(struct map* map,int nummap)
{
  	int height=map->height;
  	int width=map->width;
	char *path=map_init_save(nummap);
	FILE* fichier = NULL;
	fichier = fopen(path, "w+");

	if (fichier != NULL){
		int cTmp=0;
		int i=0;
		fprintf(fichier,"%i:%i ", width, height);
		for (i=0;i<height;i++){
			int j=0;
			fprintf(fichier,"\n");
			for (j=0;j<width;j++){
				cTmp=map->grid[i*width+j];
				fprintf(fichier,"%i ", cTmp);
			}
		}
		fclose(fichier);
	}
}



char * map_init_save(int nummap) {
    char *path;
      switch (nummap)
      {
		case 0:
			path="save/map_0"; 
		break;
        case 1: 
			path="save/map_1"; 
		break;
        case 2: 
			path="save/map_2"; 
		break;
        case 3: 
			path="save/map_3"; 
		break;
        case 4: 
			path="save/map_4"; 
		break;
      }
	return path;
   }

struct map* get_map_saved (int nummap){
	char *mapp=map_init_save(nummap);
 	FILE* ptr= fopen(mapp, "r");
 	int width;
 	int height;
 	fscanf(ptr,"%d:%d", &width,&height);
 	struct map * map = map_new(width,height);
	int i, j;
	int valeur;
 	for (i = 0; i < map_get_height(map); i++) {
		for (j = 0; j < map_get_width(map); j++) {
		fscanf(ptr,"%d", &valeur);
		map->grid[j+i*width]=valeur;
	}}
	listmonster_init(map,nummap);

	return map;
}