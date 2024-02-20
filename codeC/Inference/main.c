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

  /********** Lilian : Import of the model with weights and biais **********/


  Model_t Neural_net;
  Neural_net.nb_couche = 6;
  Neural_net.couches = (Couche_t *)malloc(Neural_net.nb_couche * sizeof(Couche_t));

// "Couche 0" => Image d'entrée
  Neural_net.couches[0].nb_neurons = 1;
  Neural_net.couches[0].nb_weights = 0;
  Neural_net.couches[0].nb_bias = 0;
  Neural_net.couches[0].lines = 28;
  Neural_net.couches[0].columns = 28;
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
  Neural_net.couches[2].lines = Neural_net.couches[1].lines/Neural_net.couches[2].kernel[0];
  Neural_net.couches[2].columns = Neural_net.couches[1].columns/Neural_net.couches[2].kernel[1];
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
  Neural_net.couches[4].nb_neurons = Neural_net.couches[2].nb_neurons;
  Neural_net.couches[4].nb_weights = 0;
  Neural_net.couches[4].nb_bias = 0;
  Neural_net.couches[4].kernel[0] = 2;
  Neural_net.couches[4].kernel[1] = 2;
  Neural_net.couches[4].lines = Neural_net.couches[3].lines/Neural_net.couches[4].kernel[0];
  Neural_net.couches[4].columns = Neural_net.couches[3].columns/Neural_net.couches[4].kernel[1];
// Couche 5 => Flatten
  Neural_net.couches[5].nb_neurons = 1;
  Neural_net.couches[5].lines = Neural_net.couches[5].nb_neurons;
// Couche 6 => Dense
  Neural_net.couches[6].nb_neurons = Neural_net.couches[5].nb_neurons;
  Neural_net.couches[6].lines = Neural_net.couches[6].nb_neurons;

  import_model(&Neural_net);

  /********** Rémy : Calcul  **********/

  BMP bitmap;
  FILE* pFichier=NULL;

  pFichier=fopen("../Lecture/0_1.bmp", "rb");     //Ouverture du fichier contenant l'image
  if (pFichier==NULL) {
      printf("%s\n", "0_1.bmp");
      printf("Erreur dans la lecture du fichier\n");
  }
  LireBitmap(pFichier, &bitmap);
  fclose(pFichier);               //Fermeture du fichier contenant l'image

  ConvertRGB2Gray(&bitmap);

/*      keras.Input(shape=input_shape),
        layers.Conv2D(32, kernel=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool=(2, 2)),
        layers.Conv2D(64, kernel=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool=(2, 2)),
        layers.Flatten(),
        layers.Dense(test_nb, activation="softmax"), */


  //Convolution 2D
  Conv2D_t Conv2D_1;
  Conv2D_1.nb = 32;
  Conv2D_1.kernel[0] = 3;
  Conv2D_1.kernel[1] = 3;
  Conv2D_1.activation = 'r';

    // Allocation de mémoire pour le résultat de la convolution 2D numéro 1
    double*** Conv2D_1_datas = (double***)malloc(Conv2D_1.nb * sizeof(double**));
    for (int i = 0; i < Conv2D_1.nb; ++i) {
        Conv2D_1_datas[i] = (double**)malloc(26 * sizeof(double*));
        for (int j = 0; j < 26; ++j) {
            Conv2D_1_datas[i][j] = (double*)malloc(26 * sizeof(double));
        }
    }

  // printf("Taille de l'image chargée en x : [%d]\n", bitmap.infoHeader.largeur);


  
  /* Couche 1 */
  printf("Traitement couche 1 : Convolution 2D\n");
  Conv2D(&bitmap, &Conv2D_1, &Neural_net.couches[0], Conv2D_1_datas);
  printf("Fin de traitement couche 1 : Convolution 2D\n");

  // Affichage des résultats de la couche 1 (Convolution 2D)
/*
  int nb_cases = 0;
  for (int neuron=0; neuron<32; neuron++)
  {
      for (int ligne=0; ligne<26; ligne++)
      {
        for (int colonne=0; colonne<26; colonne++)
        {
          printf("Result neuron %d, case %d : %.20f\n", neuron, ligne+colonne, Conv2D_1_datas[neuron][ligne][colonne]);
          nb_cases++;
        }
      }
  }
*/
  // printf("Nombre de valeurs : %d\n", nb_cases);

  //Max Pooling
  Maxpool_t Max_pooling_1;
  Max_pooling_1.nb = Conv2D_1.nb;
  Max_pooling_1.kernel[0] = 2;
  Max_pooling_1.kernel[1] = 2;
  Max_pooling_1.lines = 26/Max_pooling_1.kernel[0];
  Max_pooling_1.columns = 26/Max_pooling_1.kernel[1];  

  // Allocation de mémoire pour le résultat du max_poolong 1
  double*** max_pooling_1_datas = (double***)malloc(Conv2D_1.nb * sizeof(double**));
  for (int i = 0; i < Conv2D_1.nb; ++i) {
      max_pooling_1_datas[i] = (double**)malloc(Max_pooling_1.lines * sizeof(double*));
      for (int j = 0; j < 26; ++j) {
          max_pooling_1_datas[i][j] = (double*)malloc(Max_pooling_1.columns * sizeof(double));
      }
  }

  /* Debug couche 1 */
  // printf("Debug couche 1 : Conv2D(32, kernel=(3, 3), activation=relu)\n");
  // debug_couche1(&bitmap, &Conv2D_1, &Neural_net.couches[0], Conv2D_1_datas);


  //Max_pooling
  printf("Traitement couche 2 : Max_pooling\n");
  MaxPooling2D(Conv2D_1_datas, Max_pooling_1, max_pooling_1_datas);
  printf("Fin de traitement couche 2 : Max_pooling\n");

    // Libérer Conv2D_1_datas
    for (int i = 0; i < Conv2D_1.nb; ++i) {
        for (int j = 0; j < 26; ++j) {
            free(Conv2D_1_datas[i][j]);
        }
        free(Conv2D_1_datas[i]);
    }
    free(Conv2D_1_datas);

  printf("Libération de la mémoire de la couche convolutive 1 ==> OK\n");

  //Convolution 2D
    //TODO

  //Max_pooling
    //TODO

  // Flatten
    // TODO


  
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