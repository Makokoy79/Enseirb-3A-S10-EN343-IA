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

// Définition d'une couche
typedef struct
{                       // Chaque couche a
    int nb_neurons;     // Un nombre de neurones
    int lines;          // Un nombre de lignes en sortie
    int columns;        // Un nombre de colonnes en sortie   
    int nb_weights;     // un nombre de poids
    int nb_bias;        // un nombre de biais
    double* weights;    // Un tableau de poids
    double* bias;       // Un tableau de biais
    double*** data;     // Des données de sortie ==> A definir
    int kernel[2];      // Un "Kernel" (fenêtre de traitement)
    char activation;    // Une fonction d'activation
} Couche_t;

typedef struct
{                       // Chaque modèle a
    int nb_couche;      // Un nombre de couches
    Couche_t *couches;  // Un tableau de couches
} Model_t;

typedef struct
{
    int nb;
    int kernel[2];
    char activation;
} Conv2D_t;

typedef struct
{
    int lines;
    int columns;    
    int nb;
    int kernel[2];
} Maxpool_t;

void print_double_matrix(double* matrix, int taille);
int calcul_nb_line(FILE *file);
int calcul_nb_values_per_line(FILE *file);
void read_file(FILE *file, int nb_lines, int nb_values_per_line, double* texte);
void import_couche(Couche_t* couche, int i);
void import_model(Model_t* model);

double conv_unit(double *pixels, int nb_pixels, double weight, double bias);

// void Conv2D(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas);
void Conv2D(Couche_t* couche_in, Couche_t* couche_out);
void MaxPooling2D(double*** Conv2D_datas, Maxpool_t max_pool_shape, double*** Max_Pool_datas);
void Conv2D_2(double*** Max_Pool_datas, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas);

void debug_couche1(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas);

#endif
