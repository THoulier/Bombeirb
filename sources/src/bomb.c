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
#include <game.h>





struct listbomb *firstbomb=NULL;

void listbomb_init() {
  firstbomb = NULL;
}

void bomb_insertion(int x, int y, int range, int nummap,int etat){
  struct bomb *bomb = bomb_init(x, y, range, nummap, etat);
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

void bomb_sup(struct bomb*bomb){
  struct listbomb *prev;
  struct listbomb *next;
  struct listbomb *listbomb=firstbomb;
  if (listbomb->bomb==bomb){
    firstbomb=listbomb->next;
  }
  else{
    while (listbomb!=NULL){
      if (listbomb->next->bomb==bomb){
        prev=listbomb;
        listbomb=listbomb->next;
        next=listbomb->next;
        listbomb=NULL;
      }
      else {
            listbomb=listbomb->next;
      }
    }
    prev->next=next;
  }
  free(bomb);
}


void listbomb_refresh(struct player *player,struct map* map, struct game * game){
  struct listbomb *listbomb=firstbomb;
  while (listbomb!=NULL){
    if (bomb_start(listbomb->bomb,map,player,listbomb->bomb->etat) && listbomb->bomb->etat != 4){
      bomb_display(listbomb->bomb,game);
    }
    else if (bomb_start(listbomb->bomb,map,player,listbomb->bomb->etat) && listbomb->bomb->etat == 4){
      bomb_display(listbomb->bomb,game);
    }
    else{
      explo_end(map,listbomb->bomb->x,listbomb->bomb->y);
      for (int i=1;i<=player->bombrange;i++){
        if(listbomb->bomb->x+i<=map_get_width(map)-1){
          explo_end(map,listbomb->bomb->x+i,listbomb->bomb->y);
        }
        if(listbomb->bomb->x-i>=0){
          explo_end(map,listbomb->bomb->x-i,listbomb->bomb->y);
        }
        if(listbomb->bomb->y+i<=map_get_height(map)-1){
          explo_end(map,listbomb->bomb->x,listbomb->bomb->y+i);
        }
        if(listbomb->bomb->y-i>=0){
          explo_end(map,listbomb->bomb->x,listbomb->bomb->y-i);
        }
      }
      if (listbomb->bomb->nummap==game_get_level(game)){ 
        box_explo(map,listbomb->bomb,player);
      }
			bomb_sup(listbomb->bomb);
		}

    listbomb=listbomb->next;
  }
  if (player->bombs == 0 && firstbomb == NULL){
    player_inc_nb_bomb(player);
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

int bomb_start(struct bomb *bomb,struct map* map,struct player*player, int etat){
	int current_time=SDL_GetTicks();

  if ((current_time - bomb->time) > 1000 && bomb->etat>0 && bomb->etat != 4){
  	bomb->etat --;
  	bomb->time=SDL_GetTicks();
  }
  else if ((current_time - bomb->time) > 1000 && bomb->etat==0){
    bomb->etat=4;
  }
  else if ((current_time - bomb->time) > 1200 && bomb->etat==4 ){
    return 0;
  }
  return 1;
}



void bomb_display(struct bomb*bomb, struct game *game){
  int x=bomb->x*SIZE_BLOC;
  int y=bomb->y*SIZE_BLOC;
  if (bomb->nummap==game_get_level(game)){ 
    if (bomb->etat >= 0 && bomb->etat != 4){
      window_display_image(sprite_get_bomb(bomb->etat), x, y);
    }
    if (bomb->etat==4){
      explo_display(bomb,game_get_nummap(game,game_get_level(game)));
    }
  }
}



void explo_display(struct bomb*bomb,struct map*map){
  int x=bomb->x;
  int y=bomb->y;
  int range=bomb->range;

  int south_blocked=1;
  int east_blocked=1;
  int north_blocked=1;
  int west_blocked=1;

  switch(map_get_cell_type(map,x,y)){
    case CELL_BOX:
      window_display_image(sprite_get_explo(),x*SIZE_BLOC, y*SIZE_BLOC);
      break;
    case CELL_SCENERY:
      break;
    case CELL_EMPTY:
      map_set_cell_type(map,x,y,CELL_EXPLOSION);
      break;
    case CELL_MONSTER:
      map_set_cell_type(map,x,y,CELL_EXPLOSION);
      break;
    case CELL_EXPLOSION:
      window_display_image(sprite_get_explo(),x*SIZE_BLOC, y*SIZE_BLOC);
      break;
    default:
      break;
  }

  for(int i=1;i<=range;i++){
    if (east_blocked){
      if(x+i<=map_get_width(map)-1){
        switch(map_get_cell_type(map,x+i,y)){
          case CELL_BOX:
            if (east_blocked){
              east_blocked=0;
              window_display_image(sprite_get_explo(),(x+i)*SIZE_BLOC, y*SIZE_BLOC);
            }
            break;
          case CELL_SCENERY:
            east_blocked=0;
            break;
          case CELL_DOOR:
            east_blocked=0;
            break;
          case CELL_EMPTY:
            map_set_cell_type(map,x+i,y,CELL_EXPLOSION);
            break;
          case CELL_MONSTER:
            map_set_cell_type(map,x+i,y,CELL_EXPLOSION);
            break;
          case CELL_EXPLOSION:
            window_display_image(sprite_get_explo(),(x+i)*SIZE_BLOC, y*SIZE_BLOC);
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
              window_display_image(sprite_get_explo(),(x-i)*SIZE_BLOC, y*SIZE_BLOC);
            }
            break;
          case CELL_SCENERY:
            west_blocked=0;
            break;
          case CELL_DOOR:
            west_blocked=0;
            break;
          case CELL_EMPTY:
            map_set_cell_type(map,x-i,y,CELL_EXPLOSION);
            break;
          case CELL_MONSTER:
            map_set_cell_type(map,x-i,y,CELL_EXPLOSION);
            break;
          case CELL_EXPLOSION:
            window_display_image(sprite_get_explo(),(x-i)*SIZE_BLOC, y*SIZE_BLOC);
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
              window_display_image(sprite_get_explo(),x*SIZE_BLOC, (y+i)*SIZE_BLOC);
            }
            break;
          case CELL_SCENERY:
            south_blocked=0;
            break;
          case CELL_DOOR:
            south_blocked=0;
            break;
          case CELL_EMPTY:
            map_set_cell_type(map,x,y+i,CELL_EXPLOSION);
            break;
          case CELL_MONSTER:
            map_set_cell_type(map,x,y+i,CELL_EXPLOSION);
            break;
          case CELL_EXPLOSION:
            window_display_image(sprite_get_explo(),x*SIZE_BLOC, (y+i)*SIZE_BLOC);
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
              window_display_image(sprite_get_explo(),x*SIZE_BLOC, (y-i)*SIZE_BLOC);
            }
            break;
          case CELL_SCENERY:
            north_blocked=0;
            break;
          case CELL_DOOR:
            north_blocked=0;
            break;
          case CELL_EMPTY:
            map_set_cell_type(map,x,y-i,CELL_EXPLOSION);
            break;
          case CELL_MONSTER:
            map_set_cell_type(map,x,y-i,CELL_EXPLOSION);
            break;
          case CELL_EXPLOSION:
            window_display_image(sprite_get_explo(),x*SIZE_BLOC, (y-i)*SIZE_BLOC);
            break;
          default:
            break;
        }
      }
    }
  }
}



void explo_end(struct map* map,int x,int y){
  switch(map_get_cell_type(map,x,y)){
    case CELL_EXPLOSION:
      map_set_cell_type(map,x,y,CELL_EMPTY);
      break;
    default:
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




