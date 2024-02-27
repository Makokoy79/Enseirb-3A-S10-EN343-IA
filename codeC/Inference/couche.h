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

#define PARAM_FOLDER "./Parametres/"
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
    double*** data;     // Des données de sortie ==> Réserver espace mémoire !!!
    int kernel[2];      // Un "Kernel" (fenêtre de traitement)
    char activation;    // Une fonction d'activation
} Couche_t;

typedef struct
{                       // Chaque modèle a
    int nb_couche;      // Un nombre de couches
    Couche_t *couches;  // Un tableau de couches
} Model_t;

void pause();
int is_file_auth(char *argv1);
void print_double_matrix(double* matrix, int taille);
int calcul_nb_line(FILE *file);
int calcul_nb_values_per_line(FILE *file);
void read_file(FILE *file, int nb_lines, int nb_values_per_line, double* texte);
void import_couche(Couche_t* couche, int i);
void import_model(Model_t* model);
void Alloc_memory_datas(Couche_t* couche_in);
void Free_memory_datas(Couche_t *couche_in);

void Conv2D(Couche_t* couche_in, Couche_t* couche_out);

void MaxPooling2D(Couche_t* couche_in, Couche_t* couche_out);

void flatten(Couche_t* couche_in, Couche_t* couche_out);

void dense(Couche_t* couche_in, Couche_t* couche_out);

#endif
