#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>








void fileMonsterSave(int x,int y,enum direction direction,int nummap) //fct to add one bomb to the .txt, needs the number of bomb
{
  char *path="save/listmonster";
  FILE* fichierTmp = NULL;
  fichierTmp = fopen(path, "a"); // ouverture en mode ajout

  if (fichierTmp != NULL){
    fprintf(fichierTmp,"%u %u %u %u ",x,y,direction,nummap);
    fprintf(fichierTmp,"\n");
    fclose(fichierTmp);
  }
}

void fileNbMonsterSave(int nbMonster)
{
  char *path="save/listmonster";
  FILE* fichierTmp = NULL;
  fichierTmp = fopen(path, "w+"); // ouverture en mode ajout

  if (fichierTmp != NULL){
    fprintf(fichierTmp,"%u ",nbMonster);
    fprintf(fichierTmp,"\n");
    fclose(fichierTmp);
  }
}