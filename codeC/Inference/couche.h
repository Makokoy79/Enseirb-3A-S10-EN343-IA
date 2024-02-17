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

#define PARAM_FOLDER "../Parametres/"
#define FILE_COUCHE "couche"
#define FILE_BIAS "biais"
#define FILE_WEIGHTS "poids"
#define FILE_EXTENSION ".txt"

typedef struct
{
    int nb_weights;
    int nb_bias;
    float* weights;
    float* bias;
} Couche_t;


typedef struct
{
    int nb_couche;
    Couche_t* couches;
} Model_t;

void print_float_matrix(float* matrix, int taille);

int calcul_nb_ligne(FILE *file);
void read_file(FILE *file, int nb_lines, float* texte);
void import_couche(Couche_t* couche, int i);
void import_model(Model_t* model);

void Conv2D(int nb_filters, int kernel_size[2], char function_activation[], Couche_t couche);
void MaxPooling2D(int pool_size[2], Couche_t couche);

#endif
