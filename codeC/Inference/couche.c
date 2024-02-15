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

float conv_unit(float *pixels, int nb_pixels, float weight, float bias)
{
    float conv = 0;
    for (int i=0; i<nb_pixels; i++)
    {
        conv += pixels[i];//*poids en cours
    }
    return (conv)/nb_pixels+bias; // Biais du neurone
}

void Conv2D(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche) {
    for (int neuron = 0; neuron<Conv2D_shape->nb; neuron++)
    {
        //for 
    }
    // Pour chaque neurone
}
// void Conv2D(int nb_filters, int kernel_size[2], char function_activation[], Couche_t couche) {
//     printf("Conv2D: Number of filters: %d, Kernel size: [%d, %d], Activation function: %s\n",
//            nb_filters, kernel_size[0], kernel_size[1], function_activation);
    
//     float *pixels;

//     for (int neuron=0; neuron<nb_filters; neuron++)
//     {
//             // Pour chaque ligne de la sortie

//     }
//     // Pour chaque ligne de la sortie
//     //      Pour chaque colonne de la sortie
//     //          faire une conv_unit(x_pixels)
//     sizeofoutput = entree-filtre+1
// }

void MaxPooling2D(int pool_size[2], Couche_t couche) {
    printf("MaxPooling2D: Pool size: [%d, %d]\n",
           pool_size[0], pool_size[1]);
}