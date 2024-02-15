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

typedef struct
{
    int nb_weights;
    int nb_bias;
    int* weights;
    int* bias;
} Couche_t;


typedef struct
{
    int nb_couche;
    Couche_t* couches;
} Model_t;



void Conv2D(int nb_filters, int kernel_size[2], char function_activation[], Couche_t couche);
void MaxPooling2D(int pool_size[2], Couche_t couche);

#endif
