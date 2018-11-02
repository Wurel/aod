#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#define min(a,b) (a<=b?a:b)

long IND = 0;
int STATUS = 1;
int ERREUR = 0;
int NB_MOT = 0;
int TAMPON = 0;
int STATUS_PARAGRAPHE = 1;
int PREC_TAMPON = 0; //Cette variable est utilisée pour détecter deux retour à la
//ligne consécutif

size_t taille_fichier(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

struct paragraphe
{
    int place;
    char *mot;
    struct paragraphe *suivant;
};

char * get_mot(int M, FILE* fichier, size_t taille_f){
  char *mot = malloc(M*sizeof(char));
  PREC_TAMPON = TAMPON;
  read(fichier, &TAMPON, 1);
  IND++;
  long i = 0;
  while ((TAMPON != 32)) {
    if (IND == taille_f) {
      STATUS = 0;
      STATUS_PARAGRAPHE = 0;
      NB_MOT ++;
      return mot;
    }
    if (TAMPON == 10 && PREC_TAMPON == 10) {
      STATUS_PARAGRAPHE = 0;
    }
    if (TAMPON == 10) {
      i = 0;
      NB_MOT ++;
      return mot;
    }
    //POUR LES TABS?
    if (TAMPON == 9) {
      NB_MOT ++;
      return mot;
    }
    mot[i] = TAMPON;
    i++;
    PREC_TAMPON = TAMPON;
    read(fichier, &TAMPON, 1);
    IND++;
  }
  NB_MOT ++;
  return mot;
}

long badness(char **tableau_mots, int debut, int fin, int M){
  int longueur = 0;
  for (size_t i = debut; i < fin; i++) {
    longueur += strlen(tableau_mots[i]);
    if (strlen(tableau_mots[i]) > M) {
      fprintf(stderr, "AODjustify ERROR> Le fichier possede un mot de longueur superieur a %d\
 caracteres : justification sur %d caracteres impossibe\n",\
       M, M);
       exit(1);
    }
    if(i < fin -1) longueur ++;
    //POUR LES TAB?
    for (size_t j = 0; j < strlen(tableau_mots[i]); j++) {
      if (tableau_mots[i][j] == 9) {
        longueur = longueur + 3;
      }
    }
    }
  if (longueur > M) {
    return pow(longueur, 3);
  }
  return pow((M - longueur), 3);
}

void cout_minimal1(char **tableau_mots, long *tab_coutmin, int *tab_argmin, int M, int i) {
  //On vérifie si on est à la première ligne
  int delta_in = 0;
  for (int indice = i; indice<NB_MOT; indice++) {
    delta_in += strlen(tableau_mots[indice]);
  }
  delta_in += NB_MOT - i - 1;
  if (M - delta_in >= 0) {
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

char * completion(char * ligne, int nombre_characteres, int M, int taille_dernier_mot){
  char *nv_ligne = malloc(2*M*sizeof(char));
  char *tampon = malloc(sizeof(char));
  int i = 0;
  while (nombre_characteres < M) {
    tampon = &ligne[i];
    if (*tampon == 32 && i != 0) {
      nombre_characteres ++;
      strncpy(nv_ligne, ligne, i);
      strcat(&nv_ligne[i], " ");
      strncpy(&nv_ligne[i+1], &ligne[i], nombre_characteres - i);
      strcpy(ligne, nv_ligne);
      i++;
    }
    i ++;
    if (i > M - taille_dernier_mot) {
      i = 0;
    }
  }
  return nv_ligne;
}


void justification(char **tableau_mots, int *tab_argmin, int M, FILE *sortie){
  char *ligne = malloc(M*sizeof(char));
  int debut = 0;
  int temp;
  int fin = tab_argmin[0];
  while (debut != NB_MOT-1 && debut != fin) {
    int tab = 0;
    for (size_t i = debut; i < fin; i++) {
      strcat(ligne, tableau_mots[i]);
      if(i != fin -1)strcat(ligne, " ");
    }
    int nombre_characteres = strlen(ligne);
    char *nv_ligne = malloc(M*sizeof(char));
    nv_ligne = completion(ligne, nombre_characteres + tab, M, strlen(tableau_mots[fin-1]));
    fputs(ligne, sortie);
    fputc(13, sortie);
    strcpy(ligne, "");
    temp = fin;
    debut = fin;
    fin = tab_argmin[temp];
  }
  //Est ce qu'il reste des mots ?
  for (size_t i = fin; i < NB_MOT; i++) {
    fputs(strcat(tableau_mots[i], " "), sortie);
  }
}



int main(int argc, char const *argv[]) {
  FILE* sortie = NULL;
  long norme3 = 0;
  STATUS = 1;
  assert(argc == 3);
  int taille_f = taille_fichier(argv[2]);
  int fichier = open(argv[2], O_RDONLY, 0);
  void* mmappedData = mmap(NULL, taille_f, PROT_READ, MAP_PRIVATE, fichier, 0);
  assert(mmappedData != MAP_FAILED);
  if (fichier == -1)
  {
    fprintf(stderr, "AODjustify ERROR> Le fichier d'entrée n'existe pas...\n");
    exit(1);
  }
  sortie = fopen(strcat(argv[2],".out"), "w+");
  int M = atoi(argv[1]);
  while (STATUS != 0) {
    NB_MOT =0;
    if (STATUS_PARAGRAPHE == 0) {
      fputc(10, sortie);
      fputc(10, sortie);
    }
    STATUS_PARAGRAPHE = 1;
    struct paragraphe *tete = malloc(sizeof(struct paragraphe));
    char *mot = malloc(M*sizeof(char));
    char *ligne = malloc(M*sizeof(char));
    int nombre_characteres = 0;
    struct paragraphe *para = malloc(sizeof(struct paragraphe));
    para = tete;
    para -> place = -1;
    while (STATUS_PARAGRAPHE != 0) {
        struct paragraphe *nv_para = malloc(sizeof(struct paragraphe));
        para -> suivant = nv_para;
        nv_para -> mot = malloc(sizeof(char) * M);
        nv_para -> place = para -> place + 1 ;
        nv_para -> mot = get_mot(M, fichier, taille_f);
        para = nv_para;
      }
      char *tableau_mots[NB_MOT];
      tete = tete -> suivant;
      for (size_t i = 0; i < NB_MOT; i++) {
          tableau_mots[i] = tete->mot;
          tete = tete -> suivant;
      }
      long *tab_coutmin = malloc(NB_MOT*sizeof(long));
      int *tab_argmin = malloc(NB_MOT*sizeof(int));
      for (int indice=NB_MOT-1; indice>=0; indice--) {
        cout_minimal1(tableau_mots, tab_coutmin, tab_argmin, M, indice);
      }
      norme3 += tab_coutmin[0];
      justification(tableau_mots, tab_argmin, M, sortie);
      for (size_t i = 0; i < NB_MOT; i++) {
        strcpy(tableau_mots[i], "");
      }
      free(tab_argmin);
      free(tab_coutmin);
    }
    fprintf(stderr, "AODjustify CORRECT> la norme 3 du fichier vaut %ld\n", norme3);
    close(fichier);
    fclose(sortie);
    return 0;
}
