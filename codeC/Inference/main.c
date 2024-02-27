/*******************************************************
Nom ......... : main.c
Role ........ : Programme principal executant la phase
                d'inférence d'un réseau de neurones
                artificiels

Compilation :
    - make veryclean
    - make

Pour exécuter, tapez : ./all <chiffre>_20.bmp
  => <chiffre> doit être entre 0 inclus et 9 inclus <=

  Pour afficher les résultats intermédiaires d'une couche
  ajouter en argument couche<numero>
  <numero> peut aller de 0 inclus à 5 inclus

  couche0 : image d'entrée
  couche1 : 1ère convolution
  couche2 : 1er max pooling
  couche3 : 2ème convolution
  couche4 : 2ème max pooling
  couche5 : flatten

  Pas la peine d'argument pour la couche6, les résultats
s'affichent systématiquement
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bmp2Matrix.h"
#include "couche.h"

// Début du programme
int main(int argc, char* argv[]){

  char path[20];
  strcpy(path, "./Lecture/");
  if(argc < 2 || !is_file_auth(argv[1])) {
      printf("Il faut donner un nom de fichier à traiter valide\n");
      printf("Cela doit être de la forme : %s <numero>_20.bmp\n", argv[0]);
      printf("Par exemple : %s 0_20.bmp pour traiter le fichier avec le chiffre 0\n", argv[0]);
      printf("\nLe programme va continuer à s'exécuter entraitant l'image 0_20.bmp par défaut\n\n");
      strcat(path, "0_20.bmp");
      pause();
  }else{
    strcat(path, argv[1]);
  }



  // Ouverture du fichier image traitée
  BMP bitmap;
  FILE* pFichier=NULL;

  pFichier=fopen(path, "rb");     //Ouverture du fichier contenant l'image
  if (pFichier==NULL) {
      printf("%s\n", path);
      printf("Erreur dans la lecture du fichier\n");
      return 1;
  }

  LireBitmap(pFichier, &bitmap);  // Récupération de l'image

  fclose(pFichier);               //Fermeture du fichier contenant l'image

  ConvertRGB2Gray(&bitmap);       // Conversion de l'image en niveaux de gris

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

// Couche 1 => Convolution 2D
  Neural_net.couches[1].nb_neurons = 32;
  Neural_net.couches[1].nb_weights = 0;
  Neural_net.couches[1].nb_bias = 0;
  Neural_net.couches[1].kernel[0] = 3;
  Neural_net.couches[1].kernel[1] = 3;
  Neural_net.couches[1].lines = Neural_net.couches[0].lines-Neural_net.couches[1].kernel[0]+1;
  Neural_net.couches[1].columns = Neural_net.couches[0].columns-Neural_net.couches[1].kernel[1]+1;
  Neural_net.couches[1].activation = 'R';

// Couche 2 => Max Pooling
  Neural_net.couches[2].nb_neurons = Neural_net.couches[1].nb_neurons;
  Neural_net.couches[2].nb_weights = 0;
  Neural_net.couches[2].nb_bias = 0;
  Neural_net.couches[2].kernel[0] = 2;
  Neural_net.couches[2].kernel[1] = 2;
  Neural_net.couches[2].lines = ((Neural_net.couches[1].lines-Neural_net.couches[2].kernel[0])/Neural_net.couches[2].kernel[0])+1;
  Neural_net.couches[2].columns = ((Neural_net.couches[1].columns-Neural_net.couches[2].kernel[1])/Neural_net.couches[2].kernel[1])+1;

// Couche 3 => Convolution 2D
  Neural_net.couches[3].nb_neurons = 64;
  Neural_net.couches[3].nb_weights = 0;
  Neural_net.couches[3].nb_bias = 0;
  Neural_net.couches[3].kernel[0] = 3;
  Neural_net.couches[3].kernel[1] = 3;
  Neural_net.couches[3].lines = Neural_net.couches[2].lines-Neural_net.couches[3].kernel[0]+1;
  Neural_net.couches[3].columns = Neural_net.couches[2].columns-Neural_net.couches[3].kernel[1]+1;
  Neural_net.couches[3].activation = 'R';

// Couche 4 => Max Pooling
  Neural_net.couches[4].nb_neurons = Neural_net.couches[3].nb_neurons;
  Neural_net.couches[4].nb_weights = 0;
  Neural_net.couches[4].nb_bias = 0;
  Neural_net.couches[4].kernel[0] = 2;
  Neural_net.couches[4].kernel[1] = 2;
  Neural_net.couches[4].lines = ((Neural_net.couches[3].lines-Neural_net.couches[4].kernel[0])/Neural_net.couches[4].kernel[0])+1;
  Neural_net.couches[4].columns = ((Neural_net.couches[3].columns-Neural_net.couches[4].kernel[1])/Neural_net.couches[4].kernel[1])+1;

// Couche 5 => Flatten
  Neural_net.couches[5].nb_neurons = 1;
  Neural_net.couches[5].lines = 1;
  Neural_net.couches[5].columns = Neural_net.couches[4].nb_neurons*Neural_net.couches[4].lines*Neural_net.couches[4].columns;

// Couche 6 => Dense
  Neural_net.couches[6].nb_neurons = Neural_net.couches[5].nb_neurons;
  Neural_net.couches[6].lines = Neural_net.couches[5].lines;
  Neural_net.couches[6].columns = 10;

  // Allocation mémoire pour toutes les couches
  Alloc_memory_datas(&Neural_net);

  // Trasnfert des données de l'image dans la couche 0 (valeurs divisées par 255)
  for (int line=0; line<Neural_net.couches[0].lines; line++)
  {
    for (int column=0; column<Neural_net.couches[0].columns; column++)
    {
      Neural_net.couches[0].data[0][line][column] = (double)bitmap.mPixelsGray[column][line]/255;
    }
  }

  // Libérer bitmap
  DesallouerBMP(&bitmap);

  // Je m'assure que l'image a bien été copiée dans la mémoire du réseau de neurone
  // Vérifié ==> OK
  if(argc > 2 && !strcmp(argv[2], "couche0"))
  {
    for (int line=0; line<Neural_net.couches[0].lines; line++)
    {
      for (int column=0; column<Neural_net.couches[0].columns; column++)
      {
        printf("%f\t", Neural_net.couches[0].data[0][line][column]);
      }
      printf("\n");
    }
  }

  import_model(&Neural_net);
  printf("Importation du modèle OK\n");

  // Vérification que les poids sont bien importés dans la 1ère couche de convolution 2D
  // Vérifié. Les poids sont importés correctement
/*   
  printf("nombre de neurones : %d\n",Neural_net.couches[1].nb_neurons );
  for (int neuron=0; neuron<Neural_net.couches[1].nb_neurons; neuron++)
  {
    printf("Neurone %d\t", neuron);
    for (int weight=0; weight<Neural_net.couches[1].nb_weights; weight++)
    {
      printf("%f\t", Neural_net.couches[1].weights[neuron*Neural_net.couches[1].nb_weights+weight]);
    }
    printf("\n");
  }
 */
  // Vérification que les poids sont bien importés dans la 1ère couche de convolution 2D
  // Vérifié ==> OK
/*   
  printf("nombre de neurones : %d\n",Neural_net.couches[3].nb_neurons );
  for (int neuron=0; neuron<Neural_net.couches[3].nb_neurons; neuron++)
  {
    printf("Neurone %d\t", neuron);
    for (int weight=0; weight<Neural_net.couches[3].nb_weights; weight++)
    {

      printf("%f\t", Neural_net.couches[1].weights[neuron*Neural_net.couches[1].nb_weights+weight]);
    }
    printf("\n");
  }
 */

  /********** Rémy : Calcul  **********/

  /* Couche 1 */
  Conv2D(&Neural_net.couches[0], &Neural_net.couches[1]);
  printf("Fin de traitement couche 1 : Convolution 2D\n");

  // Vérification de la sortie de la convolution 2D pour chaque neurone
  if(argc > 2 && !strcmp(argv[2], "couche1"))
  {
    for (int neuron = 0;neuron<Neural_net.couches[1].nb_neurons; neuron++)
    {
      printf("Neurone %d\n", neuron);
      pause();
      for (int line=0; line<Neural_net.couches[1].lines; line++)
      {
        for (int column=0; column<Neural_net.couches[1].columns; column++)
        {
          printf("%f ", Neural_net.couches[1].data[neuron][line][column]);
        }
      }
      printf("\n");
    }
  }

  //Max_pooling
  MaxPooling2D(&Neural_net.couches[1], &Neural_net.couches[2]);
  printf("Fin de traitement couche 2 : Max_pooling\n");

  // Vérification de la sortie du max pooling pour le neurone x à saisir manuellement
  if(argc > 2 && !strcmp(argv[2], "couche2"))
  {
    for (int neuron = 0; neuron<Neural_net.couches[2].nb_neurons; neuron++)
    {
      printf("Neuron n°%d : \n", neuron);
      for (int line=0; line<Neural_net.couches[2].lines; line++)
      {
        for (int column=0; column<Neural_net.couches[2].columns; column++)
        {
          printf("%f ", Neural_net.couches[2].data[neuron][line][column]);
        }
      }
      printf("\n");
    }
  }

  //Convolution 2D 2
  /* Couche 1 */
  Conv2D(&Neural_net.couches[2], &Neural_net.couches[3]);
  printf("Fin de traitement couche 3 : Convolution 2D\n");
  // Vérification de la sortie de la convolution 2D pour le neurone x à saisir manuellement
  if(argc > 2 && !strcmp(argv[2], "couche3"))
  {
    for (int neuron = 0; neuron<Neural_net.couches[3].nb_neurons; neuron++)
    {
      printf("Neuron n°%d : \n", neuron);
      for (int line=0; line<Neural_net.couches[3].lines; line++)
      {
        for (int column=0; column<Neural_net.couches[3].columns; column++)
        {
          printf("%f ", Neural_net.couches[3].data[neuron][line][column]);
        }
      }
      printf("\n");
    }
  }

  //Max_pooling 2
  MaxPooling2D(&Neural_net.couches[3], &Neural_net.couches[4]);
  printf("Fin de traitement couche 4 : Max_pooling\n");
  // Vérification de la sortie du max pooling pour le neurone x à saisir manuellement
  if(argc > 2 && !strcmp(argv[2], "couche4"))
  {
    for (int neuron=0; neuron<Neural_net.couches[4].nb_neurons; neuron++)
    {
      printf("Neurone n°%d : \n", neuron);
      for (int line=0; line<Neural_net.couches[4].lines; line++)
      {
        for (int column=0; column<Neural_net.couches[4].columns; column++)
        {
          printf("%f ", Neural_net.couches[4].data[neuron][line][column]);
        }
      }
      printf("\n");
    }
  }

  // Flatten
  flatten(&Neural_net.couches[4], &Neural_net.couches[5]);
  printf("Fin de traitement couche 5 : Flatten\n");
  if(argc > 2 && !strcmp(argv[2], "couche5"))
  {
    for (int donnee=0; donnee<Neural_net.couches[5].columns; donnee++)
    {
      printf("%f\n", Neural_net.couches[5].data[0][0][donnee]);
    }
  }

  // Dense
  dense(&Neural_net.couches[5], &Neural_net.couches[6]);
  printf("Fin de traitement couche 6 : dense\n\n");
  if(argc > 2 && !strcmp(argv[2], "couche5"))
  {
    printf("Couche 6 : résultats\n");
    for (int donnee=0; donnee<Neural_net.couches[6].columns; donnee++)
    {
      printf("%f\n", Neural_net.couches[6].data[0][0][donnee]);
    }
  }

  // Calcul du résultat
  double max = 0;
  int result = 0;
  printf("\nRésultats :\n");
  for (int classe=0; classe<Neural_net.couches[6].columns; classe++)
  {
    double score = Neural_net.couches[6].data[0][0][classe]*100;
    printf("Classe %d : %6.2f%% ", classe, score);
    for (int etoile=0; etoile<(int)(score); etoile++)
    {
      printf("#");
    }
    printf("\n");    
    if (classe == 0)
    {
      max = score;
    }else if (score > max){
      max = score;
      result = classe;
    }
  }
  
  printf("\n\n*************************************************************************\n");
  printf("*** Il est écrit %d sur l'image traitée ", result);
  printf("avec une probabilité de %.2f%% ***\n", max);
  printf("*************************************************************************\n");


  /********** All free **********/
  Free_memory_datas(&Neural_net);

  return 0;

}

