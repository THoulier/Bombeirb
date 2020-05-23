#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>


/* Function save: save map/player/list of bombs/list of monsters parameters in a file .txt
Functions used when the player press S*/

void save_map(char *path){
  int height;
  int width;
  FILE* filetosave = NULL;
  FILE* file_save = NULL;
  char * path_save = "save/map";
  filetosave = fopen(path, "r"); // the file with the map modified 
  file_save = fopen(path_save, "w+"); // the file where we copy the map modified

  if (filetosave != NULL){
    fscanf(filetosave,"%u:%u ", &width, &height);
    fprintf(file_save,"%u:%u ", width, height);
    unsigned int casenum=0;
    for (int i=0;i<height;i++){
      fprintf(file_save,"\n");
      for (int j=0;j<width;j++){
        fscanf(filetosave,"%u ", &casenum);
        fprintf(file_save,"%u ", casenum);
      }
    }
    fclose(filetosave);
    fclose(file_save);
  }
}


void save_player(int x,int y, int lives, enum direction direction, int level, int bombrange, int key, int bombs, int contact, int dmg_tmp){
  char *path="save/player";
  FILE* file = NULL;
  file = fopen(path, "w+"); 

  if (file != NULL){
    fprintf(file,"%u %u %u %u %u %u %u %u %u %u ",x,y,lives,direction,level,bombrange,key,bombs,contact,dmg_tmp);
    fprintf(file,"\n");
    fclose(file);
  }
}


void save_bomb(int x,int y,enum etats etat,int nummap, int range){
  char *path="save/listbomb";
  FILE* file = NULL;
  file = fopen(path, "a"); 

  if (file != NULL){
    fprintf(file,"%u %u %u %u %u",x,y,etat,nummap,range);
    fprintf(file,"\n");
    fclose(file);
  }
}

void save_numbomb(int numbomb){
  char *path="save/listbomb";
  FILE* file = NULL;
  file = fopen(path, "w+"); 

  if (file != NULL){
    fprintf(file,"%u ",numbomb);
    fprintf(file,"\n");
    fclose(file);
  }
}



void save_monster(int x,int y,enum direction direction,int nummap){
  char *path="save/listmonster";
  FILE* file = NULL;
  file = fopen(path, "a"); 

  if (file != NULL){
    fprintf(file,"%u %u %u %u ",x,y,direction,nummap);
    fprintf(file,"\n");
    fclose(file);
  }
}

void save_nummonster(int nummonster){
  char *path="save/listmonster";
  FILE* file = NULL;
  file = fopen(path, "w+"); 

  if (file != NULL){
    fprintf(file,"%u ",nummonster);
    fprintf(file,"\n");
    fclose(file);
  }
}

/*Function load: read the player/list of bombs/list of monsters parameters written in a file with the functions save.
And assigns those parameters to load the game saved.
Functions used in the function game_load()*/
void load_listmonster(){
  char *path="save/listmonster";
  FILE* fichierTmp = NULL;
  listmonster_null();
  fichierTmp = fopen(path, "r"); 

  if (fichierTmp != NULL){

    int nummonster=0;
    fscanf(fichierTmp,"%u ",&nummonster);

    int x,y,nummap;
    enum direction direction;

    for (int i=0;i<nummonster;i++){
      fscanf(fichierTmp,"%u %u %u %u",&x,&y,&direction,&nummap);
      monster_append(x,y,nummap,direction);
    }
    fclose(fichierTmp);
  }
}

void load_listbomb(){
  char *path="save/listbomb";
  FILE* fichierTmp = NULL;
  listbomb_init();
  fichierTmp = fopen(path, "r"); 


  if (fichierTmp != NULL){

    int numbomb=0;
    fscanf(fichierTmp,"%u ",&numbomb);

    int x,y,nummap,etat,range;

    for (int i=0;i<numbomb;i++){
      fscanf(fichierTmp,"%u %u %u %u %u",&x,&y,&etat,&nummap,&range);
      bomb_insertion(x,y,range,nummap,etat);
      }
    fclose(fichierTmp);
  }
}

void load_player(struct player * player){
  char *path="save/player";
  FILE* fichierTmp = NULL;
  fichierTmp = fopen(path, "r"); 


  if (fichierTmp != NULL){

    int x,y,lives,nummap,key,bombrange,bombs,contact,dmg_tmp;
    enum direction direction;

    fscanf(fichierTmp,"%u %u %u %u %u %u %u %u %u %u",&x,&y,&lives,&direction,&nummap,&bombrange,&key,&bombs,&contact,&dmg_tmp);
    player->x=x;
    player->y=y;
    player->lives=lives;
    player->direction= NORTH;
    player->level=nummap;
    player->key=key;
    player->bombrange=bombrange;
    player->bombs=bombs;
    player->contact=contact;
    player->dmg_tmp=dmg_tmp;
    fclose(fichierTmp);
  }
}




