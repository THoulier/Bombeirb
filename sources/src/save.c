#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>



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


void load_listmonster() 
{
  char *path="save/listmonster";
  FILE* fichierTmp = NULL;
  listmonster_null();
  fichierTmp = fopen(path, "r"); 

  if (fichierTmp != NULL){

    int nummonster=0;
    fscanf(fichierTmp,"%u ",&nummonster);

    int i=0;
    int x;
    int y;
    enum direction direction;
    int nummap;

    for (i=0;i<nummonster;i++){
      fscanf(fichierTmp,"%u %u %u %u",&x,&y,&direction,&nummap);
      monster_append(x,y,nummap,direction);
    }
    fclose(fichierTmp);
  }
}

void load_listbomb() 
{
  char *path="save/listbomb";
  FILE* fichierTmp = NULL;
  listbomb_init();
  fichierTmp = fopen(path, "r"); 


  if (fichierTmp != NULL){

    int numbomb=0;
    fscanf(fichierTmp,"%u ",&numbomb);

    int i=0;
    int x;
    int y;
    int nummap;
    int etat;
    int range;

    for (i=0;i<numbomb;i++){
      fscanf(fichierTmp,"%u %u %u %u %u",&x,&y,&etat,&nummap,&range);
      bomb_insertion(x,y,range,nummap,etat);
      }
    fclose(fichierTmp);
  }
}

void load_player(struct player * player) 
{
  char *path="save/player";
  FILE* fichierTmp = NULL;
  fichierTmp = fopen(path, "r"); 


  if (fichierTmp != NULL){

    int x;
    int y;
    int lives;
    enum direction direction;
    int nummap;
    int key;
    int bombrange;
    int bombs;
    int contact;
    int dmg_tmp;

    fscanf(fichierTmp,"%u %u %u %u %u %u %u %u %u %u",&x,&y,&lives,&direction,&nummap,&key,&bombrange,&bombs,&contact,&dmg_tmp);
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




void save_map(char *path)// fct pour maj de carte contenu de path copié dans npath
{
  int height;
  int width;
  // Transfet des données initial vers .txt tmp
  FILE* fichierI = NULL;
  FILE* fichierTmp = NULL;
  char * npath = "save/map";
  fichierI = fopen(path, "r");
  fichierTmp = fopen(npath, "w+"); // ouverture en mode r+w et suppr avant

  if (fichierI != NULL){
    fscanf(fichierI,"%u:%u ", &width, &height);
    fprintf(fichierTmp,"%u:%u ", width, height);
    unsigned int caseTmp=0;
    int i=0;
    for (i=0;i<height;i++){
      fprintf(fichierTmp,"\n");
      int j=0;
      for (j=0;j<width;j++){
        fscanf(fichierI,"%u ", &caseTmp);
        fprintf(fichierTmp,"%u ", caseTmp);
      }
    }
    fclose(fichierI);
    fclose(fichierTmp);
  }
}