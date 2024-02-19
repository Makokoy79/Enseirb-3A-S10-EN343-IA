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

void print_float_matrix(float* matrix, int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%f\t", matrix[i]);

        // Revenir à la ligne tous les 5 éléments
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

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

void read_file(FILE *file, int nb_lines, float* texte) {
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char buffer[1000];

    for (int i = 0; i < nb_lines; i++) {
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            // printf("Texte %d : %s\n", i + 1, buffer);
            texte[i] = atof(buffer);
            // printf("Couche %d : %10f\n", i + 1, texte[i]);
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
    strcat(weights_filename, FILE_WEIGHTS);
    strcat(weights_filename, FILE_EXTENSION);
    printf("Filename: %s\n", weights_filename);

    strcpy(bias_filename, PARAM_FOLDER);
    strcat(bias_filename, FILE_COUCHE);
    strcat(bias_filename, num_couche);
    strcat(bias_filename, "-");
    strcat(bias_filename, FILE_BIAS);
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
        couche->nb_weights = nb_lines;
    }

    couche->weights = (float *)malloc(couche->nb_weights * sizeof(float));

    read_file(weights_file, couche->nb_weights, couche->weights);

    // printf("Matrix of weights:\n");
    // print_float_matrix(couche->weights, couche->nb_weights);

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

    couche->bias = (float *)malloc(couche->nb_bias * sizeof(float));
    
    read_file(bias_file, couche->nb_bias, couche->bias);

    // printf("Matrix of bias:\n");
    // print_float_matrix(couche->bias, couche->nb_bias);
    
    free(weights_filename);
    free(bias_filename);

    fclose(weights_file);
    fclose(bias_file);
}

void import_model(Model_t* model) {
    // lire des fichiers
        // lire un fichier
            // calculer le nombre de lignes
            // stocker les lignes dans un tableau

    for (int i = 0; i < model->nb_couche; i++)
    {
        import_couche(&model->couches[i], i);
        // printf("\n");
        // printf("Weights\n");
        // print_float_matrix(model->couches->weights, model->couches->nb_weights);
        // printf("\n");
        // printf("Bias\n");
        // print_float_matrix(model->couches->bias, model->couches->nb_bias);
        printf("\n\n");
    }
    
}



float conv_unit(float *pixels, int nb_pixels, float weight, float bias)
{
    float conv = 0;
    for (int i=0; i<nb_pixels; i++)
    {
        conv += pixels[i];//*poids en cours
    }
    return (conv)/nb_pixels+bias; // Biais du neurone
}

void Conv2D(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche, float*** Conv2D_1_datas) {
    //Pour chaque neurone à traiter
    for (int neuron = 0; neuron<Conv2D_shape->nb; neuron++)
    {
        // Pour chaque ligne de la donnée d'entrée
        for (int line = 0; line<(pBitmap->infoHeader.xResolution-Conv2D_shape->kernel_size[0]+1); line++)
        {
            // Et pour chaque colonne d ela données d'entrée
            for (int column = 0; column<(pBitmap->infoHeader.yResolution-Conv2D_shape->kernel_size[1]+1); column++)
            {
                float add_pixels = 0;
                for (int window_x = 0; window_x<Conv2D_shape->kernel_size[0]; window_x++)
                {
                    for (int window_y = 0; window_y<Conv2D_shape->kernel_size[1]; window_y++)
                    {
                        add_pixels += (pBitmap->mPixels[window_x][window_y])*(couche->weights[window_x+window_y]);
                    }
                }
                Conv2D_1_datas[neuron][line][column] += couche->bias[column+line];
            }
        }
    }
}
