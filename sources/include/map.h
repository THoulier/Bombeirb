/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_







enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70,	   		// 	0111 0000
	CELL_EXPLOSION=0x80
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};


enum scenery_type {
	SCENERY_STONE = 1,    // 0001
	SCENERY_TREE  = 2,    // 0010
	SCENERY_PRINCESS = 4  // 0100
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

//     CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,
//     CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
// 	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
//     CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
//     CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,
//     CELL_BOX_MONSTER  = CELL_BOX | BONUS_MONSTER,
// 	//for the bonus types
// 	CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC  ,
//   CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC ,
// 	CELL_BONUS_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC  ,
//   CELL_BONUS_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC  ,
//   CELL_BONUS_LIFE     = CELL_BONUS| BONUS_LIFE  ,
//   CELL_BONUS_MONSTER  = CELL_BONUS | BONUS_MONSTER  ,
// // //for keep the bonus durinf the explo
// 	CELL_EXPLO_RANGEINC = CELL_EXPLOSION | BONUS_BOMB_RANGE_INC  ,
//   CELL_EXPLOSION_RANGEDEC = CELL_EXPLOSION | BONUS_BOMB_RANGE_DEC ,
// 	CELL_EXPLOSION_BOMBINC  = CELL_EXPLOSION | BONUS_BOMB_NB_INC  ,
//   CELL_EXPLOSION_BOMBDEC  = CELL_EXPLOSION | BONUS_BOMB_NB_DEC  ,
//   CELL_EXPLOSION_LIFE     = CELL_EXPLOSION| BONUS_LIFE  ,
//   CELL_EXPLOSION_MONSTER  = CELL_EXPLOSION | BONUS_MONSTER  ,

};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
struct map* get_map (int nummap);
// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display the map on the screen
void map_display(struct map* map);
void map_set_compose_cell_type(struct map* map, int x, int y, enum compose_type type);
enum compose_type map_get_compose_type(struct map* map,int x, int y);
char * map_init(int nummap);
#endif /* MAP_H_ */
