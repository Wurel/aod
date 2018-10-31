#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#define min(a,b) (a<=b?a:b)

int STATUS = 1;
int ERREUR = 0;
int NB_MOT = 0;

struct paragraphe
{
    int place;
    char *mot;
    struct paragraphe *suivant;
};

char * get_mot(int M, FILE *fichier){
  char *mot = malloc(M*sizeof(char));
  int tampon;
  int i = 0;
  while ((tampon=fgetc(fichier)) != 32) {
    if (tampon == EOF) {
      STATUS = 0;
      NB_MOT ++;
      return mot;
    }
    if (tampon == 13) {
      printf("une entree\n");
      NB_MOT ++;
      return mot;
    }
    mot[i] = tampon;
    i++;
  }
  NB_MOT ++;
  return mot;
}

long cout(struct paragraphe *para, int debut, int fin, int M){
  for (size_t i = 0; i < debut + 1; i++) {
    para = para-> suivant;
  }
  int longueur = 0;
  for (size_t i = debut; i < fin; i++) {
    if (para == NULL) {
      printf("attention c'est nul\n");
      exit(1);
    }
    // printf("%s\n", para->mot);
    longueur += strlen(para -> mot);
    para = para -> suivant;
    if(i < fin -1) longueur ++;
  }
  // printf("On a une longueur de %d \n", longueur);
  if (longueur > M) {
    return pow(longueur, 3);
  }
  return pow((M - longueur), 3);
}

long cout_minimal(struct paragraphe *para, int i, int M, long mini){
  while (para -> place != i && para -> place < i) {
    para = para -> suivant;
    // printf("suivant %d %d \n", para->place, i);
  }
  // printf("une iterat\n");
  for (size_t j = 1; j < NB_MOT-para->place - 1; j++) {
    printf("j %ld %d\n", j, NB_MOT-para->place);
    mini = min((cout(para, 0, j, M) + cout_minimal(para, j, M, mini)), mini);
  }
  return mini;
}




int main(int argc, char const *argv[]) {
  FILE* fichier = NULL;
  FILE* sortie = NULL;
  STATUS = 1;
  struct paragraphe *tete = malloc(sizeof(struct paragraphe));

  assert(argc == 3);
  fichier = fopen(argv[2], "r");
  sortie = fopen(strcat(argv[2],".out"), "w+");
  int M = atoi(argv[1]);
  char *mot = malloc(M*sizeof(char));
  char *ligne = malloc(M*sizeof(char));
  int nombre_characteres = 0;
  if (fichier == NULL)
  {
    printf("ntm\n");
    exit(1);
  }
  struct paragraphe *para = malloc(sizeof(struct paragraphe));
  para = tete;
  para -> place = -1;
  while (STATUS != 0) {
    struct paragraphe *nv_para = malloc(sizeof(struct paragraphe));
    para -> suivant = nv_para;
    nv_para -> mot = malloc(sizeof(char) * M);
    nv_para -> place = para -> place + 1 ;
    nv_para -> mot = get_mot(M, fichier);
    printf("%s %d\n", nv_para -> mot, nv_para -> place);
    para = nv_para;
  }
  // printf("%s\n", tete -> suivant -> mot);
  // printf("%ld\n", cout(tete, 1, 3, M));
  // printf("nb mots %d\n", NB_MOT);


  printf("je comprends rien\n" );
  printf("le cout minimal est %ld\n", cout_minimal(tete->suivant, 5, M, INT_MAX));

  return 0;
}
