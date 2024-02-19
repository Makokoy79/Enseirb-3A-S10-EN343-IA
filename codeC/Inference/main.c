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

  /********** Import of the model with weights and biais **********/

  Model_t Neural_net;
  Neural_net.nb_couche = 7;
  Neural_net.couches = (Couche_t *)malloc(Neural_net.nb_couche * sizeof(Couche_t));

  import_model(&Neural_net);


  /********** Calcul  **********/

  BMP bitmap;
  FILE* pFichier=NULL;

  pFichier=fopen("0_1.bmp", "rb");     //Ouverture du fichier contenant l'image
  if (pFichier==NULL) {
      printf("%s\n", "0_1.bmp");
      printf("Erreur dans la lecture du fichier\n");
  }
  LireBitmap(pFichier, &bitmap);
  fclose(pFichier);               //Fermeture du fichier contenant l'image

  ConvertRGB2Gray(&bitmap);

/*      keras.Input(shape=input_shape),
        layers.Conv2D(32, kernel_size=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Conv2D(64, kernel_size=(3, 3), activation="relu"),
        layers.MaxPooling2D(pool_size=(2, 2)),
        layers.Flatten(),
        layers.Dense(test_nb, activation="softmax"), */

  //Convolution 2D
  Conv2D_t Conv2D_1;
  Conv2D_1.nb = 32;
  Conv2D_1.kernel_size[0] = 3;
  Conv2D_1.kernel_size[1] = 3;
  Conv2D_1.activation = 'r';

/* Allocation mémoire*/

    // Alloue de la mémoire pour x neurones de la couche
    float*** Conv2D_1_datas = malloc(Conv2D_1.nb * sizeof(float**));
    if (Conv2D_1_datas == NULL) {
        perror("Allocation de mémoire a échoué");
        exit(EXIT_FAILURE);
    }

    // Alloue de la mémoire pour chaque espace de 26x26 float (taille de la sortie)
    for (int i = 0; i < Conv2D_1.nb; i++) {
        Conv2D_1_datas[i] = malloc(bitmap.infoHeader.xResolution * sizeof(float*)); // Allouer un tableau de 26 pointeurs vers float
        if (Conv2D_1_datas[i] == NULL) {
            perror("Allocation de mémoire a échoué");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < 26; j++) {
            Conv2D_1_datas[i][j] = malloc(bitmap.infoHeader.xResolution * sizeof(float)); // Allouer un tableau de 26 float
            if (Conv2D_1_datas[i][j] == NULL) {
                perror("Allocation de mémoire a échoué");
                exit(EXIT_FAILURE);
            }
        }
    }

  Conv2D(&bitmap, &Conv2D_1, &Neural_net.couches[0]);
    //TODO

  //Max_pooling
    //TODO

  // Libérer la mémoire des données d'entrée de la fonction précédente
      for (int i = 0; i < Conv2D_1.nb; i++) {
        for (int j = 0; j < bitmap.infoHeader.xResolution; j++) {
            free(Conv2D_1_datas[i][j]);
        }
        free(Conv2D_1_datas[i]);
    }
    free(Conv2D_1_datas);
  //Convolution 2D
    //TODO

  //Max_pooling
    //TODO

  // Flatten
    // TODO


  
  /********** All free **********/

  // Free couche
  for (int i = 0; i < Neural_net.nb_couche; i++) {
      free(Neural_net.couches[i].weights);
      free(Neural_net.couches[i].bias);
  }
  free(Neural_net.couches);
  
  DesallouerBMP(&bitmap);


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