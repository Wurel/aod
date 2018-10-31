#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int STATUS = 1;
int ERREUR = 0;
int NB_MOT = 0;

void compte_mot(int M, FILE *fichier){
  int tampon;
  int i = 0;
  while ((tampon=fgetc(fichier)) != 32) {
    if (tampon == EOF) {
      STATUS = 0;
      NB_MOT ++;
    }
    if (tampon == 13) {
      NB_MOT ++;
    }
  }
  NB_MOT ++;
}

char * get_mot(int M, FILE *fichier){
  char *mot = malloc(M*sizeof(char));
  int tampon;
  int i = 0;
  while ((tampon=fgetc(fichier)) != 32) {
    if (tampon == EOF) {
      STATUS = 0;
      return mot;
    }
    if (tampon == 13) {
      return mot;
    }
    mot[i] = tampon;
    i++;
  }
  return mot;
}

int main(int argc, char const *argv[]) {
  FILE* fichier = NULL;
  FILE* sortie = NULL;
  STATUS = 1;
  assert(argc == 3);
  fichier = fopen(argv[2], "r");
  sortie = fopen(strcat(argv[2],".out"), "w+");
  int M = atoi(argv[1]);
  if (fichier == NULL)
  {
    exit(1);
  }
  while (STATUS != 0) {
    nv_para -> mot = get_mot(M, fichier);
  }
}
