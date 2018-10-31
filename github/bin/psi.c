#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <string.h>

//On crée des variables globales qui sont des tableaux
//Le premier contient les valeurs ascii de tout les caractere de fichier in
//Le second contient toutes les valeur de la fonction psi qui leur sont associé

int fonction_psi(int i, int m);
void creation_tab_psi(int m);
void creation_tab_char(char * nom_fichier);
void nombre_characteres(char * nom_fichier);

int NB_CHAR = 0;
int * TAB_CHAR = NULL;
int * TAB_PSI = NULL;

void nombre_characteres(char * nom_fichier) {
  //Cette fonction a pour but de determiner le nombre de char dans le fichier in
  FILE* fichier = NULL;
  fichier = fopen(nom_fichier, "r");
  int tampon;
  while ((tampon=fgetc(fichier)) != EOF) {
    NB_CHAR += 1;
  }
  fclose(fichier);
}

void creation_tab_char(char * nom_fichier) {
  //On cherche a creer un tableau contenant tous les caracteres du fichier d'entrée
  //En argument il y a le nom du fichier et le nombre de caractere qu'il contient
  TAB_CHAR = malloc(NB_CHAR*sizeof(int));
  FILE* fichier = NULL;
  fichier = fopen(nom_fichier, "r");
  int tampon;
  int i = 0;
  while ((tampon=fgetc(fichier)) != EOF) {
    TAB_CHAR[i] = tampon;
    i++;
  }
  fclose(fichier);
}

void creation_tab_psi(int m) {
  TAB_PSI = malloc(NB_CHAR*sizeof(int));
  for (int indice=NB_CHAR-1; indice>=0; indice--) {
    fonction_psi(indice, m);
    //printf("%d\n", TAB_PSI[indice]);
  }
}

int fonction_psi(int i, int m) {
  //fonction psi de la correction des profs, solution 2 Q2
  //Elle prend en argument la position dans le tableau du character d'entree
  //On s'occupe déjà du cas nul
  if (i == NB_CHAR-1) {
    return 0;
  }
  int k = i;
  while (k<=i+m && k<=NB_CHAR) {
    printf("je suis la\n");
    if (TAB_CHAR[k] == 26) { //26 c'est end of file
      return 0;
    }
    k++;
  }
  int min = 0; //initialisation temporaire de la valeur min
  int valeur_tempo = 0;
  for (int j=i; j<=i+m; j++) {
    if (TAB_CHAR[j] == 32) { //On vérifie la condition F(k) isspace
      if (j == i) {
        min = TAB_PSI[j+1] + (m-(j-i))*(m-(j-i))*(m-(j-i)); //initialisation de min
      }
      valeur_tempo = TAB_PSI[j+1] + (m-(j-i))*(m-(j-i))*(m-(j-i));
      if (valeur_tempo < min) {
        min = valeur_tempo;
      }
    }
  }
  return min;
}

int main(int argc, char const *argv[]) {
  assert(argc == 3);
  printf("______________________\n");
  printf("______________________\n");
  printf("______________________\n");
  nombre_characteres(argv[2]);
  printf("  -- Fonction nombre_characteres OK -- Valeur : %d \n", NB_CHAR);
  creation_tab_char(argv[2]);
  printf("  -- Fonction creation_tab_char OK \n");
  for (int indice=0; indice<NB_CHAR; indice++) {
    printf("%d; ", TAB_CHAR[indice]);
  }
  printf("\n");
  printf("______________________\n");
  printf("______________________\n");
  creation_tab_psi(argv[1]);
  printf("  -- Fonction creation_tab_psi OK \n");
  for (int indice=0; indice<NB_CHAR; indice++) {
    printf("%d; ", TAB_PSI[indice]);
  }
  free(TAB_PSI);
  free(TAB_CHAR);
  printf("______________________\n");
  printf("______________________\n");
  printf("______________________\n");
}
