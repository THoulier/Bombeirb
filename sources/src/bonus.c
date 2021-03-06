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



enum compose_type get_compose_type(enum bonus_type bonus){
  return CELL_BOX|bonus;
}

enum bonus_type get_bonus_type(enum compose_type box){
  return (box & 15);
}


void box_explo(struct map * map,struct bomb * bomb,struct player * player){ 
  /*manage the explosion of a box*/
  int x= bomb->x;
  int y= bomb->y;
  int range = player->bombrange;

  /*used to block the explosion effects in each directions*/
  int south_blocked=1;
  int east_blocked=1;
  int north_blocked=1;
  int west_blocked=1;

  if (map_get_cell_type(map,x,y) == CELL_BOX){
    enum bonus_type box=rand()%7;
    map_set_cell_type(map,x,y,get_compose_type(box));
    bonus_assign_box(map,x,y,bomb->nummap);
  }

  for (int i=1;i<range+1;i++){
    if (east_blocked){
      if(x+i<=map_get_width(map)-1){
        switch(map_get_cell_type(map,x+i,y)){
          case CELL_BOX:
            if (east_blocked){
              east_blocked=0;

              enum bonus_type box=rand()%7;
              map_set_cell_type(map,x+i,y,get_compose_type(box));
              bonus_assign_box(map,x+i,y,bomb->nummap);
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

              enum bonus_type box=rand()%7;
              map_set_cell_type(map,x-i,y,get_compose_type(box));
              bonus_assign_box(map,x-i,y,bomb->nummap);
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

              enum bonus_type box=rand()%7;
              map_set_cell_type(map,x,y+i,get_compose_type(box));
              bonus_assign_box(map,x,y+i,bomb->nummap);
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

              enum bonus_type box=rand()%7;
              map_set_cell_type(map,x,y-i,get_compose_type(box));
              bonus_assign_box(map,x,y-i,bomb->nummap);
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


void bonus_assign_box(struct map *map,int x,int y, int nummap){
  /*assign the good cell type to a box according to the bonus type configured before*/
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
