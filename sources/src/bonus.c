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
#include <monster.h>




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
  int south_blocked=1;
  int east_blocked=1;
  int north_blocked=1;
  int west_blocked=1;

  if (map_get_cell_type(map,x,y) == CELL_BOX){
    bonus_config(map,x,y);
    display_bonus_explo(map,x,y,player_get_level(player));
  }
  for (int i=1;i<range+1;i++){
    if (east_blocked){
      if(x+i<=map_get_width(map)-1){
        switch(map_get_cell_type(map,x+i,y)){
          case CELL_BOX:
            if (east_blocked){
              east_blocked=0;
              bonus_config(map,x+i,y);
              display_bonus_explo(map,x+i,y,player_get_level(player));
            }
          break;
          case CELL_SCENERY:
            east_blocked=0;
          break;
          case CELL_DOOR:
            east_blocked=0;
          break;
          case CELL_BONUS:
            east_blocked=0;
            break;
          default:

            break;
        }
      }
    }
    if (west_blocked){
      if(x-i>=0){
        switch(map_get_cell_type(map,x-i,y)){
          case CELL_BOX:
            if (west_blocked){
              west_blocked=0;
              bonus_config(map,x-i,y);
              display_bonus_explo(map,x-i,y,player_get_level(player));
            }
          break;
          case CELL_SCENERY:
            west_blocked=0;
          break;
          case CELL_DOOR:
            west_blocked=0;
          break;
          case CELL_BONUS:
            west_blocked=0;
            break;

          default:

            break;
        }
      }
    }
    if (south_blocked){
      if(y+i<=map_get_height(map)-1){
        switch(map_get_cell_type(map,x,y+i)){
          case CELL_BOX:
            if (south_blocked){
              south_blocked=0;
              bonus_config(map,x,y+i);
              display_bonus_explo(map,x,y+i,player_get_level(player));
            }
          break;
          case CELL_SCENERY:
            south_blocked=0;
          break;
          case CELL_DOOR:
            south_blocked=0;
          break;
          case CELL_BONUS:
            south_blocked=0;
            break;

          default:

            break;
        }
      }
    }
    if (north_blocked){
      if(y-i>=0){
        switch(map_get_cell_type(map,x,y-i)){
          case CELL_BOX:
            if (north_blocked){
              north_blocked=0;
              bonus_config(map,x,y-i);
              display_bonus_explo(map,x,y-i,player_get_level(player));
            }
          break;
          case CELL_SCENERY:
            north_blocked=0;
          break;
          case CELL_DOOR:
            north_blocked=0;
          break;
          case CELL_BONUS:
            north_blocked=0;
            break;

          default:

            break;
        }
      }
    }
  }
}


void display_bonus_explo(struct map *map,int x,int y, int nummap){
  enum bonus_type bonus=get_bonus_type(map_get_compose_type(map,x,y));
  if (bonus==0){
    map_set_cell_type(map,x,y,CELL_EMPTY);
  }
  else if (bonus==5){
    map_set_cell_type(map,x,y,CELL_BONUS|bonus);
    monster_append(x,y,nummap,SOUTH);
  }
  else {
    map_set_cell_type(map,x,y,CELL_BONUS|bonus);
  }

}
