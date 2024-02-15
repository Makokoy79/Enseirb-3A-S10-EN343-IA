/*******************************************************
Nom ......... : couche.h
Role ........ : Codage des couches du réseau
Version ..... : /
Licence ..... : /

Compilation : /
Pour exécuter, tapez : /
********************************************************/

#ifndef COUCHE_H
#define COUCHE_H

#include "Bmp2Matrix.h"

typedef struct
{
    int lines;
    int columns;
} Shape_t;

typedef struct
{
    int nb_weights;
    int nb_bias;
    int *weights;
    int *bias;
} Couche_t;

typedef struct
{
    int nb_couche;
    Couche_t *couches;
} Model_t;

  typedef struct
{
    int nb;
    int kernel_size[2];
    char activation;
} Conv2D_t;

  typedef struct
{
    int pool_size[2];
} MaxPool_t;

//void Conv2D(int nb_filters, int kernel_size[2], char function_activation[], Couche_t couche);

void Conv2D(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche);

void MaxPooling2D(int pool_size[2], Couche_t couche);

#endif
