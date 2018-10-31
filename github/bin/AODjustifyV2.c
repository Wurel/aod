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
  int tampon = fgetc(fichier);
  int i = 0;
  while ((tampon != 32)) {
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
    tampon=fgetc(fichier);
  }
  NB_MOT ++;
  return mot;
}

long badness(char **tableau_mots, int debut, int fin, int M){
  int longueur = 0;
  for (size_t i = debut; i < fin; i++) {
    // printf("%s\n", para->mot);
    longueur += strlen(tableau_mots[i]);
    if(i < fin -1) longueur ++;
  }
  // printf("On a une longueur de %d \n", longueur);
  if (longueur > M) {
    return pow(longueur, 3);
  }
  return pow((M - longueur), 3);
}

long cout_minimal(struct paragraphe *para, int i, int M, long mini){
  if (i+1 == NB_MOT) {
    printf("tandam\n");
    return 0;
  }
  while (para -> place != i && para -> place < i) {
    para = para -> suivant;
    printf("suivant %d %d \n", para->place, i);
  }
  printf("mot de depart %s\n", para->mot);
  for (size_t j = 1; j < NB_MOT-para->place  ; j++) {
    printf("j %ld %d\n", j, NB_MOT-para->place-1);
    mini = min(badness(para, 0, j, M) + cout_minimal(para, j, M, mini), mini);
  }
  return mini;
}

void cout_minimal1(char **tableau_mots, long *tab_coutmin, int *tab_argmin, int M, int i) {
  if (i == NB_MOT - 1) {
    //Si on cherche le cout minimal du dernier mot
    tab_coutmin[i] = 0;
    tab_argmin[i] = i;
    return;
  }
  //On initialise la valeur du min des j à j = i+1
  long min = tab_coutmin[i+1]+badness(tableau_mots, i, i+1, M);
  int argmin = i;
  long val_tempo = 0;
  for (int j=i+1; j<NB_MOT; j++) {
    val_tempo = tab_coutmin[j]+badness(tableau_mots, i, j, M);
    if (val_tempo < min) {
      //On actualise la valeur du minimum
      argmin = j;
      min = val_tempo;
    }
  }
  tab_coutmin[i] = min;
  tab_argmin[i] = argmin;
}


void justification(char **tableau_mots, int *tab_argmin, int M, FILE *sortie){
  printf("debut\n" );
  char *ligne = malloc(M*sizeof(char));
  int debut = 0;
  int temp;
  int fin = tab_argmin[0];
  while (fin != NB_MOT-1) {
    for (size_t i = debut; i < fin; i++) {
      strcat(ligne, tableau_mots[i]);
      strcat(ligne, " ");
    }
    // printf("%d\n", fin);
    // int nombre_characteres = strlen(ligne);
    // int nombres_espaces = M - strlen(ligne);
    // printf("%d\n", nombre_characteres);
    // strcpy(ligne, "");
    // printf("ta mere\n" );
    // while (nombre_characteres < M) {
    //   for (size_t i = debut; i < fin; i++) {
    //     // printf("on ajoute un mpt %s\n", tableau_mots[i]);
    //     strcat(ligne, tableau_mots[i]);
    //     strcat(ligne, " ");
    //     if (nombres_espaces > 0) {
    //       strcat(ligne, " ");
    //       nombres_espaces--;
    //       nombre_characteres ++;
    //       // printf("on rajoute un  \n");
    //     }
    //   }
    // }
    // fprintf(stderr, "%s\n", ligne);
    // for (size_t i = 0; i <= M; i++) {
    //   fputc(ligne[i], sortie);
    //   // printf("%c\n", ligne[i]);
    // }
    // fputc(13, sortie);
    fprintf(stderr, "%s\n", ligne);
    fputs(ligne, sortie);
    fputc(13, sortie);
    strcpy(ligne, "");
    temp = fin;
    debut = fin;
    fin = tab_argmin[temp];
  }
  printf("fin\n");
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
  char *tableau_mots[NB_MOT];
  // struct paragraphe *para = malloc(sizeof(struct paragraphe));
  // para2 = tete;
  tete = tete -> suivant;
  for (size_t i = 0; i < NB_MOT-1; i++) {
    // struct paragraphe *nv_para = malloc(sizeof(struct paragraphe));
    // para -> suivant = nv_para;
    // tableau_mots[i] = malloc(sizeof(char)*strlen(tete->mot));
    tableau_mots[i] = tete->mot;
    printf("%s\n", tete -> suivant ->mot );
    // para = nv_para;
    tete = tete -> suivant;
  }
  // printf("%s\n", tete -> suivant -> mot);
  // printf("%ld\n", cout(tete, 1, 3, M));
  printf("nb mots %d\n", NB_MOT);
  for (size_t i = 1; i < NB_MOT; i++) {
    printf("%s\n", tableau_mots[i]);
  }
  long *tab_coutmin = malloc(NB_MOT*sizeof(long));
  int *tab_argmin = malloc(NB_MOT*sizeof(int));
  // printf("je comprends rien\n" );
  for (int indice=NB_MOT-1; indice>=0; indice--) {
    cout_minimal1(tableau_mots, tab_coutmin, tab_argmin, M, indice);
  }
  printf("le cout minimal est %ld\n", tab_coutmin[0]);
  justification(tableau_mots, tab_argmin, M, sortie);

//   for (size_t i = 0; i < NB_MOT; i++) {
// printf("%d\n", tab_argmin[i]);  }
  fclose(fichier);
  // fclose(sortie);

  return 0;
}

//la phrase 2 elle commence a argmin(0) la deuxieme argmin(8)
