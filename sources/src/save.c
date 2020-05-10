#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>






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
