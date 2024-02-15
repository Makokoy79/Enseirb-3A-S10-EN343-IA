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

int main(int argc, char* argv[]){
  printf("Au travail!\n");
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