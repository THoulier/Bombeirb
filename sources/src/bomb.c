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
#include <save.h>




struct listbomb *firstbomb=NULL;

void listbomb_init() {
  firstbomb = NULL;
}

void bomb_insertion(int x, int y, int range, int nummap,int etat){
  struct bomb *bomb = bomb_init(x, y, range, nummap,etat);
  if (firstbomb == NULL) {
    firstbomb = malloc(sizeof(*firstbomb));
    firstbomb->bomb = bomb;
    firstbomb->next = NULL;
    return;
  }
  struct listbomb *listbomb = malloc(sizeof(*listbomb));
  listbomb->bomb=bomb;
  listbomb->next=firstbomb;
  firstbomb=listbomb;
}


void listbomb_refresh(struct player *player,struct map* map, struct game * game){
  struct listbomb *listbomb=firstbomb;
  while (listbomb!=NULL){
    if (bomb_start(listbomb->bomb,map,player) && listbomb->bomb->etat!=4){
			bomb_display(listbomb->bomb,game);
			}
    if (bomb_start(listbomb->bomb,map,player) && listbomb->bomb->etat==4){
      box_explo(map,listbomb->bomb,player);
      //explo_display(listbomb->bomb,player,map);

    }
    listbomb=listbomb->next;

  }
  }


struct bomb * bomb_init(int x, int y, int range, int nummap, int etat){ 
  struct bomb * bomb=malloc(sizeof(*bomb));
  bomb->x=x;
  bomb->y=y;
  bomb->time=SDL_GetTicks();
  bomb->range=range;
  bomb->nummap=nummap;
  bomb->etat=etat;
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
      if(bomb->x+i<=map_get_width(map)){
        if(map_get_cell_type(map,bomb->x+i,bomb->y)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x+i,bomb->y,CELL_EMPTY);}
      }      
      if(bomb->x-i>=0){
        if(map_get_cell_type(map,bomb->x-i,bomb->y)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x-i,bomb->y,CELL_EMPTY);}
      }
      if(bomb->y+i<=map_get_height(map)){
        if(map_get_cell_type(map,bomb->x,bomb->y+i)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x,bomb->y+i,CELL_EMPTY);}
      }
      if(bomb->y-i>=0){
        if(map_get_cell_type(map,bomb->x,bomb->y-i)==CELL_EXPLOSION){
          map_set_cell_type(map,bomb->x,bomb->y-i,CELL_EMPTY);}
      }
    }
    map_set_cell_type(map,bomb->x,bomb->y,CELL_EMPTY);
  }*/
  else{
    return 0;
  }
  return 1;
}



void bomb_display(struct bomb*bomb, struct game *game){
  int x=bomb->x*SIZE_BLOC;
  int y=bomb->y*SIZE_BLOC;
  if (bomb->nummap==game_get_level(game)){ 
    window_display_image(sprite_get_bomb(bomb->etat), x, y);
  }

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


void listbomb_pause(int time_pause){
  struct listbomb *listbomb=firstbomb;

  while (listbomb!=NULL){
    listbomb->bomb->time=listbomb->bomb->time+time_pause;
    listbomb=listbomb->next;
  }

}

int listbomb_get_length(struct listbomb * listbomb){
	int numbomb=0;
	listbomb=firstbomb;
	while(listbomb){
		numbomb +=1;
		listbomb=listbomb->next;
	}
	return numbomb;
}


void listbomb_save(){
  struct listbomb *listbomb=firstbomb;
	save_numbomb(listbomb_get_length(listbomb));

	while(listbomb){
		save_bomb(listbomb->bomb->x,listbomb->bomb->y,listbomb->bomb->etat,listbomb->bomb->nummap, listbomb->bomb->range);
		listbomb=listbomb->next;
	}

}




