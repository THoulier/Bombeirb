#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>








void fileMonsterSave(int x,int y,enum direction direction,int nummap){
  char *path="save/listmonster";
  FILE* fichierTmp = NULL;
  fichierTmp = fopen(path, "a"); 

  if (fichierTmp != NULL){
    fprintf(fichierTmp,"%u %u %u %u ",x,y,direction,nummap);
    fprintf(fichierTmp,"\n");
    fclose(fichierTmp);
  }
}

void fileNbMonsterSave(int nbMonster){
  char *path="save/listmonster";
  FILE* fichierTmp = NULL;
  fichierTmp = fopen(path, "w+"); 

  if (fichierTmp != NULL){
    fprintf(fichierTmp,"%u ",nbMonster);
    fprintf(fichierTmp,"\n");
    fclose(fichierTmp);
  }
}
