/*******************************************************
Nom ......... : main.c
Role ........ : Programme principal executant la phase
                d'inférence d'un rédeau de neurones
                artificiels

Compilation :
make veryclean
make
Pour exécuter, tapez : ./all
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bmp2Matrix.h"
#include "couche.h"

int main(int argc, char* argv[]){
  // Ouverture du fichier image traitée
  BMP bitmap;
  FILE* pFichier=NULL;

  pFichier=fopen("../Lecture/0_20.bmp", "rb");     //Ouverture du fichier contenant l'image
  if (pFichier==NULL) {
      printf("%s\n", "0_20.bmp");
      printf("Erreur dans la lecture du fichier\n");
  }
  LireBitmap(pFichier, &bitmap);
  fclose(pFichier);               //Fermeture du fichier contenant l'image
  ConvertRGB2Gray(&bitmap);

  // Permet d'afficher les valeurs de chaque pixel de l'image importée.
  // Pratique pour faire les calculs à la main et vérifier les résultats d'un traitement
  // Pour debug seulement
  /*
  for (int line=0; line<bitmap.infoHeader.hauteur; line++)
  {
    for (int column=0; column<bitmap.infoHeader.largeur; column++)
    {
      printf("%d\t", bitmap.mPixelsGray[line][column]);
    }
    printf("\n");
  }
  */

  /********** Lilian : Import of the model with weights and biais **********/

  Model_t Neural_net;
  Neural_net.nb_couche = 7;
  Neural_net.couches = (Couche_t *)malloc(Neural_net.nb_couche * sizeof(Couche_t));

// "Couche 0" => Image d'entrée
// Couche vérifiée, les données de l'image à traiter ont bien été copiées dans la mémoire
  Neural_net.couches[0].nb_neurons = 1;
  Neural_net.couches[0].nb_weights = 0;
  Neural_net.couches[0].nb_bias = 0;
  Neural_net.couches[0].lines = bitmap.infoHeader.hauteur;
  Neural_net.couches[0].columns = bitmap.infoHeader.largeur;
  // Allocation de mémoire pour l'image
  Neural_net.couches[0].data = (double***)malloc(Neural_net.couches[0].nb_neurons * sizeof(double**));
  for (int i = 0; i < Neural_net.couches[0].lines; ++i) {
      Neural_net.couches[0].data[i] = (double**)malloc(Neural_net.couches[0].lines * sizeof(double*));
      for (int j = 0; j < Neural_net.couches[0].lines; ++j) {
          Neural_net.couches[0].data[i][j] = (double*)malloc(Neural_net.couches[0].columns * sizeof(double));
      }
  }
  // Récupération des données de l'image
  for (int line=0; line<Neural_net.couches[0].lines; line++)
  {
    for (int column=0; column<Neural_net.couches[0].columns; column++)
    {
      Neural_net.couches[0].data[0][line][column] = (double)bitmap.mPixelsGray[line][column]/255;
    }
  }
  // Libérer bitmap
  DesallouerBMP(&bitmap);


// Couche 1 => Convolution 2D
  Neural_net.couches[1].nb_neurons = 32;
  Neural_net.couches[1].nb_weights = 0;
  Neural_net.couches[1].nb_bias = 0;
  Neural_net.couches[1].kernel[0] = 3;
  Neural_net.couches[1].kernel[1] = 3;
  Neural_net.couches[1].lines = Neural_net.couches[0].lines-Neural_net.couches[1].kernel[0]+1;
  Neural_net.couches[1].columns = Neural_net.couches[0].columns-Neural_net.couches[1].kernel[1]+1;
  Neural_net.couches[1].activation = 'R';
  // Allocation de mémoire pour le résultat de la convolution 2D numéro 1
  Neural_net.couches[1].data = (double***)malloc(Neural_net.couches[1].nb_neurons * sizeof(double**));
  for (int i = 0; i < Neural_net.couches[1].nb_neurons; ++i)
  {
      Neural_net.couches[1].data[i] = (double**)malloc(Neural_net.couches[1].lines * sizeof(double*));
      for (int j = 0; j < Neural_net.couches[1].lines; ++j) {
          Neural_net.couches[1].data[i][j] = (double*)malloc(Neural_net.couches[1].columns * sizeof(double));
      }
  }

// Couche 2 => Max Pooling
  Neural_net.couches[2].nb_neurons = Neural_net.couches[1].nb_neurons;
  Neural_net.couches[2].nb_weights = 0;
  Neural_net.couches[2].nb_bias = 0;
  Neural_net.couches[2].kernel[0] = 2;
  Neural_net.couches[2].kernel[1] = 2;
  Neural_net.couches[2].lines = ((Neural_net.couches[1].lines-Neural_net.couches[2].kernel[0])/Neural_net.couches[2].kernel[0])+1;
  Neural_net.couches[2].columns = ((Neural_net.couches[1].columns-Neural_net.couches[2].kernel[1])/Neural_net.couches[2].kernel[1])+1;
  // Allocation de mémoire pour le résultat du max pooling numéro 1
  Neural_net.couches[2].data = (double***)malloc(Neural_net.couches[2].nb_neurons * sizeof(double**));
  for (int i = 0; i < Neural_net.couches[2].nb_neurons; ++i)
  {
      Neural_net.couches[2].data[i] = (double**)malloc(Neural_net.couches[2].lines * sizeof(double*));
      for (int j = 0; j < Neural_net.couches[2].lines; ++j) {
          Neural_net.couches[2].data[i][j] = (double*)malloc(Neural_net.couches[2].columns * sizeof(double));
      }
  }
  
// Couche 3 => Convolution 2D
  Neural_net.couches[3].nb_neurons = 64;
  Neural_net.couches[3].nb_weights = 0;
  Neural_net.couches[3].nb_bias = 0;
  Neural_net.couches[3].kernel[0] = 3;
  Neural_net.couches[3].kernel[1] = 3;
  Neural_net.couches[3].lines = Neural_net.couches[2].lines-Neural_net.couches[3].kernel[0]+1;
  Neural_net.couches[3].columns = Neural_net.couches[2].columns-Neural_net.couches[3].kernel[1]+1;
  Neural_net.couches[3].activation = 'R';
  // Allocation de mémoire pour le résultat de la convolution 2D numéro 2
  Neural_net.couches[3].data = (double***)malloc(Neural_net.couches[3].nb_neurons * sizeof(double**));
  for (int i = 0; i < Neural_net.couches[3].nb_neurons; ++i)
  {
      Neural_net.couches[3].data[i] = (double**)malloc(Neural_net.couches[3].lines * sizeof(double*));
      for (int j = 0; j < Neural_net.couches[3].lines; ++j) {
          Neural_net.couches[3].data[i][j] = (double*)malloc(Neural_net.couches[3].columns * sizeof(double));
      }
  }
printf("couche4 debut\n");
// Couche 4 => Max Pooling
  Neural_net.couches[4].nb_neurons = Neural_net.couches[3].nb_neurons;
  Neural_net.couches[4].nb_weights = 0;
  Neural_net.couches[4].nb_bias = 0;
  Neural_net.couches[4].kernel[0] = 2;
  Neural_net.couches[4].kernel[1] = 2;
  Neural_net.couches[4].lines = ((Neural_net.couches[3].lines-Neural_net.couches[4].kernel[0])/Neural_net.couches[4].kernel[0])+1;
  Neural_net.couches[4].columns = ((Neural_net.couches[3].columns-Neural_net.couches[4].kernel[1])/Neural_net.couches[4].kernel[1])+1;
  // Allocation de mémoire pour le résultat du max pooling numéro 2
  Neural_net.couches[4].data = (double***)malloc(Neural_net.couches[4].nb_neurons * sizeof(double**));
  for (int i = 0; i < Neural_net.couches[4].nb_neurons; ++i)
  {
      Neural_net.couches[4].data[i] = (double**)malloc(Neural_net.couches[4].lines * sizeof(double*));
      for (int j = 0; j < Neural_net.couches[4].lines; ++j) {
          Neural_net.couches[4].data[i][j] = (double*)malloc(Neural_net.couches[4].columns * sizeof(double));
      }
  }

printf("couche4 fin\n");
// Couche 5 => Flatten
  Neural_net.couches[5].nb_neurons = 1;
  Neural_net.couches[5].lines = Neural_net.couches[5].nb_neurons;
  Neural_net.couches[5].columns = Neural_net.couches[4].nb_neurons*Neural_net.couches[4].lines*Neural_net.couches[4].columns;

// Couche 6 => Dense
  Neural_net.couches[6].nb_neurons = Neural_net.couches[5].nb_neurons;
  Neural_net.couches[6].lines = Neural_net.couches[6].nb_neurons;
  Neural_net.couches[6].columns = 10;

  import_model(&Neural_net);
  printf("Importation du modèle OK\n");

  // Vérification que les poids sont bien importés
  // Vérifié. Les poids sont importés correctement
/*   printf("nombre de neurones : %d\n",Neural_net.couches[1].nb_neurons );
  for (int neuron=0; neuron<Neural_net.couches[1].nb_neurons; neuron++)
  {
    printf("Neurone %d\t", neuron);
    for (int weight=0; weight<Neural_net.couches[1].nb_weights; weight++)
    {
      printf("%f\t", Neural_net.couches[1].weights[neuron*Neural_net.couches[1].nb_weights+weight]);
    }
    printf("\n");
  } */

  /********** Rémy : Calcul  **********/

  /* Couche 1 */
  Conv2D(&Neural_net.couches[0], &Neural_net.couches[1]);
  printf("Fin de traitement couche 1 : Convolution 2D\n");

  // Vérification de la sortie de la convolution 2D pour le neurone x à saisir manuellement
 /*
  int neuron = 12;
    printf("Neuron n°%d : \n", neuron);
    for (int line=0; line<Neural_net.couches[1].lines; line++)
    {
      for (int column=0; column<Neural_net.couches[1].columns; column++)
      {
        printf("%f\t", Neural_net.couches[1].data[neuron][line][column]);
      }
      printf("\n");
    }
*/

  /* Debug couche 1 */
  // printf("Debug couche 1 : Conv2D(32, kernel=(3, 3), activation=relu)\n");
  // debug_couche1(&bitmap, &Conv2D_1, &Neural_net.couches[0], Conv2D_1_datas);

  //Max_pooling
  MaxPooling2D(&Neural_net.couches[1], &Neural_net.couches[2]);
  printf("Fin de traitement couche 2 : Max_pooling\n");
  // Vérification de la sortie du max pooling pour le neurone x à saisir manuellement
/*
  int neuron = 45;
  printf("Neurone n°%d : \n", neuron);
  for (int line=0; line<Neural_net.couches[2].lines; line++)
  {
    for (int column=0; column<Neural_net.couches[2].columns; column++)
    {
      printf("%f\t", Neural_net.couches[2].data[neuron][line][column]);
    }
    printf("\n");
  }
*/

  //Convolution 2D 2
  /* Couche 1 */
  Conv2D(&Neural_net.couches[2], &Neural_net.couches[3]);
  printf("Fin de traitement couche 3 : Convolution 2D\n");
  // Vérification de la sortie de la convolution 2D pour le neurone x à saisir manuellement
/* 
  int neuron = 51;
    printf("Neuron n°%d : \n", neuron);
    for (int line=0; line<Neural_net.couches[3].lines; line++)
    {
      for (int column=0; column<Neural_net.couches[3].columns; column++)
      {
        printf("%f\t", Neural_net.couches[3].data[neuron][line][column]);
      }
      printf("\n");
    }
*/

  //Max_pooling 2
  MaxPooling2D(&Neural_net.couches[3], &Neural_net.couches[4]);
  printf("Fin de traitement couche 4 : Max_pooling\n");
  // Vérification de la sortie du max pooling pour le neurone x à saisir manuellement

  int neuron = 45;
  printf("Neurone n°%d : \n", neuron);
  for (int line=0; line<Neural_net.couches[4].lines; line++)
  {
    for (int column=0; column<Neural_net.couches[4].columns; column++)
    {
      printf("%f\t", Neural_net.couches[4].data[neuron][line][column]);
    }
    printf("\n");
  }


  // Flatten
    //TODO

  // Dense
    //TODO


  
  /********** All free **********/

  // Free model and couche
  for (int i = 0; i < Neural_net.nb_couche; i++) {
      free(Neural_net.couches[i].weights);
      free(Neural_net.couches[i].bias);
  }
  free(Neural_net.couches);

  printf("Libération de la mémoire des paramètres ==> OK\n");

  DesallouerBMP(&bitmap);

  printf("Libération de la mémoire de l'image d'entrée ==> OK\n");



  return 0;
}


//  Fonction pour récupérer tout les paramètres du réseau
//  Pour chaque couche : récupérer les poids et biais à parti des ".txt"
//  Faire des malloc pour stocker les donées

//  Application de chaque couche à partir  des paramètres
//    Ecriture des fonctions correspondant à chaque couche
//      Multiplication, Somme       => Convolution
//      Comparaison                 => Max Pooling
//       Fonction d'activation
//       Mise à plat                => Flatten
//    Ecriture des fonctions de comparaison de chaque couche afin de vérifier le résultat