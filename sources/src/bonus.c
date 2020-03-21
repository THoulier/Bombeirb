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
    enum compose_type box=bonus_set_type();
    map_set_cell_type(map,x,y,get_compose_type(box));
}

void map_bonus_init(struct map*map){ 
  int h=map_get_height(map);
  int w=map_get_width(map);
  srand(time(NULL));
  for(int i=0;i<w;i++){
    for(int j=0;j<h;j++){
      if (map_get_cell_type(map,i,j)==CELL_BOX){
        bonus_config(map,i,j);
      }
    }
  }
}

enum bonus_type bonus_set_type(){ 
  
    int ale=rand()%6;
    return ale+1;
}

enum compose_type get_compose_type(enum bonus_type bonus){
    return CELL_BOX|bonus;
}

enum bonus_type get_bonus_type(enum compose_type box){
  return (box &15);
}


void box_explo(struct map*map,struct bomb*bomb,struct player * player){ //uses for change the cell type when the box had a bonus at the end of the explo

  int x= bomb->x;
  int y= bomb->y;
  int range = player->bombrange;
  


  for (int i=1;i<range+1;i++){
  
        if (map_get_cell_type(map,x+i,y) == CELL_BOX){
            display_bonus_explo(map,x+i,y);
        }
      
        if (map_get_cell_type(map,x-i,y) == CELL_BOX){
            display_bonus_explo(map,x-i,y);
        }
        if (map_get_cell_type(map,x,y+i) == CELL_BOX){
            display_bonus_explo(map,x,y+i);
        }
        if (map_get_cell_type(map,x,y-i) == CELL_BOX){
            display_bonus_explo(map,x,y-i);
        }
      }
}


void display_bonus_explo(struct map *map,int x,int y){  
  enum bonus_type bonus=get_bonus_type(map_get_compose_type(map,x,y));
  
  
  if (bonus==0){
    map_set_cell_type(map,x,y,CELL_EMPTY);
  }
  else{
  
    map_set_cell_type(map,x,y,CELL_BONUS|bonus);
  }
}