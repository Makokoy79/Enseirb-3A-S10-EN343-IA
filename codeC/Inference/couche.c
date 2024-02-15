/*******************************************************
Nom ......... : Couche.c
Role ........ : Codage des couches du réseau
Version ..... : /
Licence ..... : /

Compilation : /
Pour exécuter, tapez : /
********************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "couche.h"

void Conv2D(int nb_filters, int kernel_size[2], char function_activation[], Couche_t couche) {
    printf("Conv2D: Number of filters: %d, Kernel size: [%d, %d], Activation function: %s\n",
           nb_filters, kernel_size[0], kernel_size[1], function_activation);
}

void MaxPooling2D(int pool_size[2], Couche_t couche) {
    printf("MaxPooling2D: Pool size: [%d, %d]\n",
           pool_size[0], pool_size[1]);
}