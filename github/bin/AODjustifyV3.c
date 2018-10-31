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
      break;
    }
    if (tampon == 13) {
      NB_MOT ++;
      break;
    }
    tampon=fgetc(fichier);
  }
  NB_MOT ++;
}

char * get_mot(int M, FILE *fichier){
  char *mot = malloc(M*sizeof(char));
  int tampon = fgetc(fichier);
  int i = 0;
  while ((tampon != 32)) {
    if (tampon == EOF) {
      STATUS = 0;
      return mot;
    }
    if (tampon == 13) {
      return mot;
    }
    mot[i] = tampon;
    i++;
    tampon=fgetc(fichier);
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
    compte_mot(M, fichier);
  }
  rewind(fichier);
  STATUS = 1;
  //On cree un tableau avec tous les mots du texte
  //Il contient NB_MOT ayant une taille inférieur ou égal à M
  //La taille des mot peut être optimisé
  int i = 0;
  char * tab_mot[NB_MOT];
  while (STATUS != 0) {
    tab_mot[i] = get_mot(M, fichier);
    i++;
  }
  for (int indice=0; indice<NB_MOT; indice++) {
    printf("%s; ", tab_mot[indice]);
  }
  printf("NB_MOT : %d\n", NB_MOT);
  return 0;
}
