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

#define PARAM_FOLDER "../Parametres/"
#define FILE_COUCHE "couche"
#define FILE_BIAS "biais"
#define FILE_WEIGHTS "poids"
#define FILE_EXTENSION ".txt"

typedef struct
{
    int lines;
    int columns;
} Shape_t;

typedef struct
{
    int nb_weights;
    int nb_bias;
    double* weights;
    double* bias;
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
    int lines;
    int columns;    
    int nb;
    int kernel_size[2];
} Maxpool_t;

void print_double_matrix(double* matrix, int taille);
int calcul_nb_ligne(FILE *file);
void read_file(FILE *file, int nb_lines, double* texte);
void import_couche(Couche_t* couche, int i);
void import_model(Model_t* model);

double conv_unit(double *pixels, int nb_pixels, double weight, double bias);

void Conv2D(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas);
void MaxPooling2D(double*** Conv2D_datas, Maxpool_t max_pool_shape, double*** Max_Pool_datas);

void debug_couche1(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas);

#endif
