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

// void bomb_sup(struct bomb*bomb){
//   struct listbomb *prec;
//   struct listbomb *suiv;
//   struct listbomb *listbomb;
//   listbomb=list;
//   if (listbomb->first==bomb){
//     list=listbomb->next;
//   }
//   else{
//     while (listbomb!=NULL){
//       if (listbomb->next->first==bomb){
//         prec=listbomb;
//         listbomb=listbomb->next;
//         suiv=listbomb->next;
//         listbomb=NULL;
//             }
//       else {
//             listbomb=listbomb->next;
//           }
//           }
//           prec->next=suiv;
//         }
//   free(bomb);
// }

void listbomb_refresh(struct player *player,struct map* map){
  struct listbomb *listbomb=list;
  while (listbomb!=NULL){
    if (bomb_start(listbomb->first,map,player) && listbomb->first->etat!=4){
			bomb_display(listbomb->first);
			}
    if (bomb_start(listbomb->first,map,player) && listbomb->first->etat==4){
      box_explo(map,listbomb->first,player);
      explo_display(listbomb->first,player,map);

    }else {
    int range = player->bombrange;
    for(int i=1;i<=range;i++){
      if(listbomb->first->x+i<=map_get_width(map)){
        if(map_get_cell_type(map,listbomb->first->x+1,listbomb->first->y)==CELL_EXPLOSION){
          map_set_cell_type(map,listbomb->first->x+1,listbomb->first->y,CELL_EMPTY);}
      }      
      if(listbomb->first->x-i>=0){
        if(map_get_cell_type(map,listbomb->first->x-1,listbomb->first->y)==CELL_EXPLOSION){
          map_set_cell_type(map,listbomb->first->x-1,listbomb->first->y,CELL_EMPTY);}
      }
      if(listbomb->first->y+i<=map_get_height(map)){
        if(map_get_cell_type(map,listbomb->first->x,listbomb->first->y+1)==CELL_EXPLOSION){
          map_set_cell_type(map,listbomb->first->x,listbomb->first->y+1,CELL_EMPTY);}
      }
      if(listbomb->first->y-i>=0){
        if(map_get_cell_type(map,listbomb->first->x,listbomb->first->y-1)==CELL_EXPLOSION){
          map_set_cell_type(map,listbomb->first->x,listbomb->first->y-1,CELL_EMPTY);}
      }
    }
    map_set_cell_type(map,listbomb->first->x,listbomb->first->y,CELL_EMPTY);
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
    bomb->etat=4;

  }
/*
  else if ((current_time-time)<4600){
    int range = player->bombrange;
    for(int i=1;i<=range;i++){
      if(bomb->x+i<map_get_width(map)){
        if(map_get_cell_type(map,bomb->x+1,bomb->y)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x+1,bomb->y,CELL_EMPTY);}
      }      
      if(bomb->x-i>=0){
        if(map_get_cell_type(map,bomb->x-1,bomb->y)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x-1,bomb->y,CELL_EMPTY);}
      }
      if(bomb->y+i<map_get_height(map)){
        if(map_get_cell_type(map,bomb->x,bomb->y+1)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x,bomb->y+1,CELL_EMPTY);}
      }
      if(bomb->y-i>=0){
        if(map_get_cell_type(map,bomb->x,bomb->y-1)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x,bomb->y-1,CELL_EMPTY);}
      }
    }
    map_set_cell_type(map,bomb->x,bomb->y,CELL_EMPTY);
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
  int x=bomb->x;
  int y=bomb->y;
  int range=player->bombrange;
  int i;

  explos(map,x,y);

  for(i=1;i<=range;i++){
    if(x+i<=map_get_width(map)){
      explos(map,x+i,y);}
    if(x-i>=0){
      explos(map,x-i,y);}
    if(y+i<=map_get_height(map)){
      explos(map,x,y+i);}
    if(y-i>=0){
      explos(map,x,y-i);}
}
}

void explos(struct map* map,int x,int y){

  switch(map_get_cell_type(map,x,y)){
        case CELL_BONUS:
          window_display_image(sprite_get_explo(),x*SIZE_BLOC, y*SIZE_BLOC);
          //map_set_compose_cell_type(map,x*SIZE_BLOC,y*SIZE_BLOC,CELL_BONUS|get_bonus_type(map_get_compose_type(map,x,y)));
        break;
        case CELL_EMPTY:
          map_set_cell_type(map,x,y,CELL_EXPLOSION);
        break;

        case CELL_MONSTER:
          map_set_cell_type(map,x,y,CELL_EXPLOSION);
          break;
        case CELL_EXPLOSION:
          //window_display_image(sprite_get_explo(),x*SIZE_BLOC, y*SIZE_BLOC);
          break;

        default:
          //window_display_image(sprite_get_explo(),x*SIZE_BLOC, y*SIZE_BLOC);

          break;

  }



}

