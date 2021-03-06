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
  /*add an initialized bomb in the list of bombs*/
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
  /*supression of a bomb in the list*/
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
  /*Refresh the action linked to a bomb in game: time, box explosion, explosion, the bomb number of the player*/
  struct listbomb *listbomb=firstbomb;
  while (listbomb!=NULL){
    /*refresh the bomb picture during the 4 seconds*/
    if (bomb_start(listbomb->bomb,map,player,listbomb->bomb->etat) && listbomb->bomb->etat != 4){
      bomb_display(listbomb->bomb,game);
    }
    /*display the explosion after the 4 seconds*/
    else if (bomb_start(listbomb->bomb,map,player,listbomb->bomb->etat) && listbomb->bomb->etat == 4){
      bomb_display(listbomb->bomb,game);
    }
    else{
      /*supress the cell type CELL_EXPLOSION from the explosion*/
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
      /*a box blow up if the bomb is in the same level*/
      box_explo(game_get_nummap(game,listbomb->bomb->nummap),listbomb->bomb,player);
      
      /*bomb supressed and bomb number refreshed*/
			bomb_sup(listbomb->bomb);
      player_inc_nb_bomb(player);
		}
    listbomb=listbomb->next;
  }
}


struct bomb * bomb_init(int x, int y, int range, int nummap, int etat){ 
  /*initialisation of a bomb parameters*/
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
  /*refresh the states of a bomb*/
	int current_time=SDL_GetTicks();

  if ((current_time - bomb->time) > 1000 && bomb->etat>0 && bomb->etat != 4){//4 seconds countdown
  	bomb->etat --;
  	bomb->time=SDL_GetTicks();
  }
  else if ((current_time - bomb->time) > 1000 && bomb->etat==0){//explosion starts
    bomb->etat=4;
  }
  else if ((current_time - bomb->time) > 1200 && bomb->etat==4 ){//explosion ends
    return 0;
  }
  return 1;
}



void bomb_display(struct bomb*bomb, struct game *game){
  /*display the bomb if its in the same level as the player*/
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
  /*manage the propagation of the explosion*/
  int x=bomb->x;
  int y=bomb->y;
  int range=bomb->range;

  /*used to block the explosion effects in each directions*/
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
    case CELL_BONUS:
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



void explo_end(struct map* map,int x,int y){
  /*supress CELL_EXPLOSION*/
  switch(map_get_cell_type(map,x,y)){
    case CELL_EXPLOSION:
      map_set_cell_type(map,x,y,CELL_EMPTY);
      break;
    default:
      break;
  }
}


void listbomb_pause(int time_pause){
  /*Refresh the bomb time when the player take a break*/
  struct listbomb *listbomb=firstbomb;

  while (listbomb!=NULL){
    listbomb->bomb->time=listbomb->bomb->time+time_pause;
    listbomb=listbomb->next;
  }
}

int listbomb_get_length(struct listbomb * listbomb){
  /*get the number of bombs in listbomb to save it in a .txt file*/
	int numbomb=0;
	listbomb=firstbomb;
	while(listbomb){
		numbomb +=1;
		listbomb=listbomb->next;
	}
	return numbomb;
}


void listbomb_save(){
  /*save the list bomb parameters*/
  struct listbomb *listbomb=firstbomb;
	save_numbomb(listbomb_get_length(listbomb));

	while(listbomb){
		save_bomb(listbomb->bomb->x,listbomb->bomb->y,listbomb->bomb->etat,listbomb->bomb->nummap, listbomb->bomb->range);
		listbomb=listbomb->next;
	}
}




