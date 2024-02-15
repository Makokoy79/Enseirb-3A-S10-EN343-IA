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
    int couche;
    int weights[];
    int biais[];
} Model_t;

void Conv2D(int nb_filters, int kernel_size[2], char function_activation[], Model_t model);
void MaxPooling2D(int pool_size[2], Model_t model);

#endif
