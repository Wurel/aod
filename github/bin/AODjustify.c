#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>

int STATUS = 1;
int ERREUR = 0;


char * get_mot(int M, FILE *fichier){
  char *mot = malloc(M*sizeof(char));
  int tampon;
  int i = 0;
  while ((tampon=fgetc(fichier)) != 32) {
    if (tampon == EOF) {
      STATUS = 0;
      return mot;
    }
    mot[i] = tampon;
    i++;
  }
  return mot;
}

char * justification(char * ligne, int nombre_characteres, int M, int taille_dernier_mot){
  printf("%s\n", ligne);
  char *nv_ligne = malloc(2*M*sizeof(char));
  char *tampon = malloc(sizeof(char));
  int i = 0;
  while (nombre_characteres < M) {
    tampon = &ligne[i];
    if (*tampon == 32 && i != 0) {
      nombre_characteres ++;
      ERREUR ++;
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

int main(int argc, char const *argv[]) {
  FILE* fichier = NULL;
  FILE* sortie = NULL;
  STATUS = 1;
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
  while(nombre_characteres <= M){
    mot = get_mot(M, fichier);
    if(nombre_characteres < M - strlen(mot)&& nombre_characteres != 0){
      ligne = strcat(ligne, " ");
      nombre_characteres ++;
    }
    if(STATUS == 0){
      ligne = strcat(ligne, mot);
      // ligne = justification(l  igne, nombre_characteres, M);
      fputs(ligne, sortie);
      // fputc(EOF, sortie);
      printf("ERREUR %d\n", ERREUR);
      fclose(fichier);
      fclose(sortie);
      return 0;
    };
    if (nombre_characteres + strlen(mot) > M) {
      ligne = justification(ligne, nombre_characteres, M, strlen(mot));
      fputs(ligne, sortie);
      fputc(13, sortie);
      nombre_characteres = 0;
      free(ligne);
      ligne = malloc(M*sizeof(char));
    }
    if (nombre_characteres + strlen(mot) == M) {
      strcat(ligne, mot);
      fputs(ligne, sortie);
      fputc(13, sortie);
      nombre_characteres = 0;
      free(ligne);
      ligne = malloc(M*sizeof(char));
      continue;
    }
    ligne = strcat(ligne, mot);
    nombre_characteres += strlen(mot);
  }
}
