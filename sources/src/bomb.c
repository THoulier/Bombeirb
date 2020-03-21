#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>
#include <player.h>
#include <constant.h>
#include <map.h>
#include <window.h>
#include <sprite.h>
#include <monster.h>
#include <bomb.h>
#include <bonus.h>



struct listbomb *list=NULL;

void listbomb_init() {
  list = NULL;
}

void bomb_insertion(int x, int y, struct map *map){
  struct bomb *bomb = bomb_init(x,y);
  //map_set_cell_type(map,x,y,CELL_BOMB);
  if (list == NULL) {
    list = malloc(sizeof(*list));
    list->first = bomb;
    list->next = NULL;
    return;
  }
  struct listbomb *listbomb = malloc(sizeof(*listbomb));
  listbomb->first=bomb;
  listbomb->next=list;
  list=listbomb;
}

void bomb_sup(struct bomb*bomb){
  struct listbomb *prec;
  struct listbomb *suiv;
  struct listbomb *listbomb;
  listbomb=list;
  if (listbomb->first==bomb){
    list=listbomb->next;
  }
  else{
    while (listbomb!=NULL){
      if (listbomb->next->first==bomb){
        prec=listbomb;
        listbomb=listbomb->next;
        suiv=listbomb->next;
        listbomb=NULL;
            }
      else {
            listbomb=listbomb->next;
          }
          }
          prec->next=suiv;
        }
  free(bomb);
}

void listbomb_refresh(struct player *player,struct map* map){
  struct listbomb *listbomb=list;
  while (listbomb!=NULL){
    if (bomb_start(listbomb->first,map,player) && listbomb->first->etat!=4){
			bomb_display(listbomb->first);
			}
    if (bomb_start(listbomb->first,map,player) && listbomb->first->etat==4){

      //explo_display(listbomb->first,player,map);

      box_explo(map,listbomb->first,player);
    }

    listbomb=listbomb->next;




  }
}

struct bomb * bomb_init(int x, int y){ //fct to create a new bomb
  struct bomb * bomb=malloc(sizeof(*bomb));
  bomb->x=x;
  bomb->y=y;
  bomb->time=SDL_GetTicks();
  bomb->etat=3;
  return(bomb);
}

int bomb_start(struct bomb *bomb,struct map* map,struct player*player){
  int time=bomb->time;
	int current_time=SDL_GetTicks();


	if ((current_time-time)<1000){
		bomb->etat=3;
  }
	else if ((current_time-time)<2000){
		bomb->etat=2;

  }

	else if ((current_time-time)<3000){
		bomb->etat=1;
  }
	else if ((current_time-time)<4000){
	   bomb->etat=0;

  }
  else if ((current_time-time)<4500){
    explo_display(bomb,player,map);

    //box_explo(map,bomb);

    //map_set_cell_type(map,bomb->x,bomb->y,CELL_EMPTY);
    bomb->etat=4;
    
  }
  /*
  else if ((current_time-time)==5000){
    
    map_set_cell_type(map,bomb->x+1,bomb->y,CELL_EXPLOSION);
    map_set_cell_type(map,bomb->x-1,bomb->y,CELL_EXPLOSION);
    map_set_cell_type(map,bomb->x,bomb->y,CELL_EXPLOSION);
    map_set_cell_type(map,bomb->x,bomb->y+1,CELL_EXPLOSION);
    map_set_cell_type(map,bomb->x,bomb->y-1,CELL_EXPLOSION);

  }*/
  else{
    return 0;
  }
  return 1;
}



void bomb_display(struct bomb*bomb){
  int x=bomb->x*SIZE_BLOC;
  int y=bomb->y*SIZE_BLOC;
  window_display_image(sprite_get_bomb(bomb->etat), x, y);


}

void explo_display(struct bomb*bomb,struct player* player,struct map*map){
  int x=bomb->x*SIZE_BLOC;
  int y=bomb->y*SIZE_BLOC;
  int range=player->bombrange;
  int i;

  for(i=1;i<=range;i++){
     int a = 1;
     int b = 1;
     int c = 1;
     int d = 1;
     int e = 1;



    
    if (a == 1){
      //if (bomb->x+i<map_get_width(map)){
    switch(map_get_cell_type(map,bomb->x+i,bomb->y)){
          case CELL_BOX:
            map_set_compose_cell_type(map,x+i,y,CELL_EXPLOSION|get_bonus_type(map_get_compose_type(map,x+i,y)));
            window_display_image(sprite_get_explo(), x+i*SIZE_BLOC, y);
          break;

          case CELL_EMPTY:
            map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            //window_display_image(sprite_get_explo(),x+i*SIZE_BLOC, y);
          break;

          case CELL_MONSTER:
            map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            break;

          case CELL_EXPLOSION:
            //map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            window_display_image(sprite_get_explo(),x+i*SIZE_BLOC, y);
          break;

          case CELL_SCENERY:
            a=0;
          break;
          default:
            a=0;
            break;
    }
      //}
    }
    if (b == 1){
    switch(map_get_cell_type(map,bomb->x-i,bomb->y)){
          case CELL_BOX:
            map_set_compose_cell_type(map,x-i,y,CELL_EXPLOSION|get_bonus_type(map_get_compose_type(map,x-i,y)));
            window_display_image(sprite_get_explo(), x-i*SIZE_BLOC, y);
          break;

          case CELL_EMPTY:
            map_set_cell_type(map,bomb->x-i,bomb->y,CELL_EXPLOSION);
            //window_display_image(sprite_get_explo(),x-i*SIZE_BLOC, y);
          case CELL_MONSTER:
            map_set_cell_type(map,bomb->x-i,bomb->y,CELL_EXPLOSION);
            break;

          break;
          case CELL_EXPLOSION:
            //map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            window_display_image(sprite_get_explo(),x-i*SIZE_BLOC, y);


          break;

          case CELL_SCENERY:
            b=0;
          break;
          default:
            b=0;
            break;
    }
    }
    if (c == 1){
    switch(map_get_cell_type(map,bomb->x,bomb->y+i)){
          case CELL_BOX:
            map_set_compose_cell_type(map,x,y+i,CELL_EXPLOSION|get_bonus_type(map_get_compose_type(map,x,y+i)));
            window_display_image(sprite_get_explo(), x, y+i*SIZE_BLOC);
          break;

          case CELL_EMPTY:
            map_set_cell_type(map,bomb->x,bomb->y+i,CELL_EXPLOSION);
            //window_display_image(sprite_get_explo(),x, y+i*SIZE_BLOC);
          case CELL_MONSTER:
            map_set_cell_type(map,bomb->x,bomb->y+i,CELL_EXPLOSION);
            break;

          break;
          case CELL_EXPLOSION:
            //map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            window_display_image(sprite_get_explo(),x, y+i*SIZE_BLOC);


          break;


          case CELL_SCENERY:
            c=0;
          break;
          default:
            c=0;
            break;
    }
    }
    if (d == 1){
    switch(map_get_cell_type(map,bomb->x,bomb->y-i)){
          case CELL_BOX:
            map_set_compose_cell_type(map,x,y-i,CELL_EXPLOSION|get_bonus_type(map_get_compose_type(map,x,y-i)));
            window_display_image(sprite_get_explo(), x, y-i*SIZE_BLOC);
          break;

          case CELL_EMPTY:
            map_set_cell_type(map,bomb->x,bomb->y-i,CELL_EXPLOSION);
            //window_display_image(sprite_get_explo(),x, y-i*SIZE_BLOC);
          case CELL_MONSTER:
            map_set_cell_type(map,bomb->x,bomb->y-i,CELL_EXPLOSION);
            break;

          break;
          case CELL_EXPLOSION:
            //map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            window_display_image(sprite_get_explo(),x, y-i*SIZE_BLOC);


          break;

          case CELL_SCENERY:
            d=0;
          break;
          default:
            d=0;
            break;
    }
    }
    if (e == 1){
    switch(map_get_cell_type(map,bomb->x,bomb->y)){
          case CELL_BOX:
            map_set_compose_cell_type(map,x,y,CELL_EXPLOSION|get_bonus_type(map_get_compose_type(map,x,y)));
            window_display_image(sprite_get_explo(), x, y);
          break;

          case CELL_EMPTY:
            map_set_cell_type(map,bomb->x,bomb->y,CELL_EXPLOSION);
            //window_display_image(sprite_get_explo(),x, y);

          case CELL_MONSTER:
            map_set_cell_type(map,bomb->x,bomb->y,CELL_EXPLOSION);
            break;
          break;
          case CELL_EXPLOSION:
            //map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EXPLOSION);
            window_display_image(sprite_get_explo(),x, y);


          break;

          case CELL_SCENERY:
            e=0;
          break;
          default:
            e=0;
            break;
    }
    }
  
 }

}


  


