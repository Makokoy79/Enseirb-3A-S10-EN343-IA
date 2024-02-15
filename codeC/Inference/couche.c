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
#include <string.h>

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

int calcul_nb_ligne(FILE *file) {
    int nb_lines = 0;
    char ch;

    // Compter le nombre de lignes
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            nb_lines++;
        }
    }

    // Rembobiner le fichier
    rewind(file);

    // printf("Number of lines: %d\n", nb_lines);
    return nb_lines;
}

void read_file(FILE *file, int nb_lines, int* texte) {
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char buffer[1000];

    for (int i = 0; i < nb_lines; i++) {
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("ALigne %d : %s\n", i + 1, buffer);
            // printf("atof('4')=%f\n",atof("4"));
            printf("atoi(buffer)=%f\n",atof(buffer));
            texte[i] = atof(buffer);
            printf("BLigne %d : %d\n", i + 1, texte[i]);
        } else {
            printf("Erreur de lecture de la ligne %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }
}


void import_couche(Couche_t* couche, int i) {
    FILE *weights_file;
    FILE *bias_file;

    char *weights_filename = NULL;
    char *bias_filename = NULL;
    char num_couche[10];

    int nb_lines = 0;

    sprintf(num_couche, "%d", i);

    weights_filename = (char *)malloc(strlen(PARAM_FOLDER) + strlen(FILE_COUCHE) + strlen(num_couche) + strlen(FILE_WEIGHTS) + strlen(FILE_EXTENSION) + 1);
    bias_filename = (char *)malloc(strlen(PARAM_FOLDER) + strlen(FILE_COUCHE) + strlen(num_couche) + strlen(FILE_BIAS) + strlen(FILE_EXTENSION) + 1);

    if (weights_filename == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    if (bias_filename == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    strcpy(weights_filename, PARAM_FOLDER);
    strcat(weights_filename, FILE_COUCHE);
    strcat(weights_filename, num_couche);
    strcat(weights_filename, "-");
    strcat(weights_filename, FILE_BIAS);
    strcat(weights_filename, FILE_EXTENSION);
    printf("Filename: %s\n", weights_filename);

    strcpy(bias_filename, PARAM_FOLDER);
    strcat(bias_filename, FILE_COUCHE);
    strcat(bias_filename, num_couche);
    strcat(bias_filename, "-");
    strcat(bias_filename, FILE_WEIGHTS);
    strcat(bias_filename, FILE_EXTENSION);
    printf("Filename: %s\n", bias_filename);

    

    /********** Weights **********/
    weights_file = fopen(weights_filename, "r");

    if (weights_file == NULL) {
        printf("No weights and bias in couche %d\n", i);
        couche->nb_weights = 0;
        couche->nb_bias = 0;
        return;
    }

    nb_lines = calcul_nb_ligne(weights_file);

    if (nb_lines == 0) {
        printf("No weights in couche %d\n", i);
        couche->nb_weights = 0;
    }
    else
    {
        printf("Number of weights in couche %d\n", nb_lines);
        couche->nb_bias = nb_lines;
    }

    read_file(weights_file, nb_lines, couche->weights);

    /********** Bias **********/
    bias_file = fopen(bias_filename, "r");

    if (bias_file == NULL) {
        printf("No bias in couche %d\n", i);
        couche->nb_bias = 0;
        return;
    }

    nb_lines = calcul_nb_ligne(bias_file);

    if (nb_lines == 0) {
        printf("No bias in couche %d\n", i);
        couche->nb_bias = 0;
    }
    else
    {
        printf("Number of bias in couche %d\n", nb_lines);
        couche->nb_bias = nb_lines;
    }

    read_file(bias_file, nb_lines, couche->bias);
    
    free(weights_filename);
    free(bias_filename);

    fclose(weights_file);
    fclose(bias_file);
}


void import_model(Model_t model) {
    // lire des fichiers
        // lire un fichier
            // calculer le nombre de lignes
            // stocker les lignes dans un tableau
}


void MaxPooling2D(int pool_size[2], Couche_t couche) {
    printf("MaxPooling2D: Pool size: [%d, %d]\n",
           pool_size[0], pool_size[1]);
}

