#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>

#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <bonus.h>
#include <game.h>
#include <player.h>
#include <map.h>




void bonus_config(struct map *map,int x, int y){
    srand(time(NULL));
    int ale=rand()%7;
    enum bonus_type box=ale;
    map_set_cell_type(map,x,y,get_compose_type(box));
}


enum compose_type get_compose_type(enum bonus_type bonus){
    return CELL_BOX|bonus;
}

enum bonus_type get_bonus_type(enum compose_type box){
  return (box & 15);
}


void box_explo(struct map * map,struct bomb * bomb,struct player * player){ 

  int x= bomb->x;
  int y= bomb->y;
  int range = player->bombrange;
  if (map_get_cell_type(map,x,y) == CELL_BOX){
    bonus_config(map,x,y);
    display_bonus_explo(map,x,y);
  }
  for (int i=1;i<range+1;i++){

        if (map_get_cell_type(map,x+i,y) == CELL_BOX){
          bonus_config(map,x+i,y);
          display_bonus_explo(map,x+i,y);
        }
        if (map_get_cell_type(map,x-i,y) == CELL_BOX){
          bonus_config(map,x-i,y);
          display_bonus_explo(map,x-i,y);
        }
        if (map_get_cell_type(map,x,y+i) == CELL_BOX ){
          bonus_config(map,x,y+i);
          display_bonus_explo(map,x,y+i);
        }
        if (map_get_cell_type(map,x,y-i) == CELL_BOX){
          bonus_config(map,x,y-i);
          display_bonus_explo(map,x,y-i);
        }
      }
}


void display_bonus_explo(struct map *map,int x,int y){
  enum bonus_type bonus=get_bonus_type(map_get_compose_type(map,x,y));
  if (bonus==0){
    map_set_cell_type(map,x,y,CELL_EMPTY);
  }
  else if (bonus==5){
    map_set_cell_type(map,x,y,CELL_BONUS|bonus);
    //monster_append(game->monster,)
  }
  else {
    map_set_cell_type(map,x,y,CELL_BONUS|bonus);
  }

}
