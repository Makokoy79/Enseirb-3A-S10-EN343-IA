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

void print_double_matrix(double* matrix, int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%f\t", matrix[i]);

        // Revenir à la ligne tous les 5 éléments
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int calcul_nb_line(FILE *file) {
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

    //printf("Number of lines: %d\n", nb_lines);
    return nb_lines;
}

int calcul_nb_values_per_line(FILE *file) {
    int values_per_line = 0;
    char ch;

    // Count the number of values in the first line
    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        if (ch == ' ') {
            values_per_line++;
        }
    }

    // Rewind the file
    rewind(file);

    // Increment by 1 to account for the last value in the line
    return values_per_line + 1;
}

void read_file(FILE *file, int nb_lines, int nb_values_per_line, double* texte) {
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char buffer[1000];

    for (int i = 0; i < nb_lines; i++) {
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            // Read the values into the allocated memory
            char *token = strtok(buffer, " ");
            int j = 0;
            while (token != NULL && j < nb_values_per_line) {
                texte[i * nb_values_per_line + j] = atof(token);
                token = strtok(NULL, " ");
                j++;
            }
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
        couche->nb_neurons = 0;
        couche->nb_weights = 0;
        couche->nb_bias = 0;
        return;
    }

    nb_lines = calcul_nb_line(weights_file);

    couche->nb_weights = calcul_nb_values_per_line(weights_file);
    printf("Number of weights per neuron %d\n", couche->nb_weights);

    if (nb_lines == 0) {
        printf("No neurons in layer %d\n", i);
        couche->nb_neurons = 0;
    }
    else
    {
        printf("Number of neurons in layer %d\n", nb_lines);
        couche->nb_neurons = nb_lines;
    }

    couche->weights = (double *)malloc(couche->nb_neurons * couche->nb_weights * sizeof(double));

    read_file(weights_file, couche->nb_neurons, couche->nb_weights, couche->weights);

    /********** Bias **********/
    bias_file = fopen(bias_filename, "r");

    if (bias_file == NULL) {
        printf("No bias in couche %d\n", i);
        couche->nb_bias = 0;
        return;
    }

    nb_lines = calcul_nb_line(bias_file);

    couche->nb_bias = calcul_nb_values_per_line(bias_file);
    printf("Number of bias per neuron %d\n", couche->nb_bias);

    if (nb_lines != couche->nb_neurons) {
        printf("Error, not the same number of neurons in weight and bias files\n");
        return;
    }
    else if (nb_lines == 0)
    {
        printf("No neurons in layer %d\n", i);
        couche->nb_neurons = 0;
    }
    
    couche->bias = (double *)malloc(couche->nb_neurons * couche->nb_bias * sizeof(double));
    
    read_file(bias_file, couche->nb_neurons, couche->nb_bias, couche->bias);
    
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

    for (int i = 1; i < model->nb_couche; i++)
    {
        import_couche(&model->couches[i], i);
        // printf("\n");
        // printf("Weights\n");
        // print_double_matrix(model->couches->weights, model->couches->nb_weights);
        // printf("\n");
        // printf("Bias\n");
        // print_double_matrix(model->couches->bias, model->couches->nb_bias);
        printf("\n\n");
    }
    
}

void Conv2D(Couche_t* couche_in, Couche_t* couche_out) {
    //Pour chaque neurone à traiter
    for (int neuron = 0; neuron<couche_out->nb_neurons; neuron++)
    {
        // Pour chaque ligne de la donnée d'entrée
        for (int line = 0; line<(couche_out->lines); line++)
        {
            // Et pour chaque colonne de la données d'entrée
            for (int column = 0; column<(couche_out->columns); column++)
            {
                for (int window_x = 0; window_x<couche_out->kernel[0]; window_x++)
                {
                    for (int window_y = 0; window_y<couche_out->kernel[1]; window_y++)
                    {
                        double weight = couche_out->weights[neuron*couche_out->nb_weights + window_x*couche_out->kernel[0]+window_y];
                        couche_out->data[neuron][line][column] += (couche_in->data[0][line+window_x][column+window_y])*weight;
                    }
                }
                (couche_out->data[neuron][line][column] += couche_out->bias[neuron]);
                // Fonction d'activation RELU
                if (couche_out->data[neuron][line][column]< 0)
                {
                    couche_out->data[neuron][line][column] = 0;
                }
            }
        }
    }
}

void debug_couche1(BMP* pBitmap, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas) {
    // FILE *inter_file;

    // int nb_lines = 0;

    // inter_file = fopen("../Parametres/inter1.txt", "r");

    // if (inter_file == NULL) {
    //     perror("Erreur d'ouverture du fichier");
    //     return;
    // }

    // nb_lines = calcul_nb_line(inter_file);

    // if (nb_lines == 0) {
    //     printf("Void file\n");
    //     return;
    // }
    // else
    // {
    //     printf("Number of lines : %d\n", nb_lines);
    // }

    // double* inter_value = (double *)malloc(nb_lines * sizeof(double));

    // read_file(inter_file, nb_lines, inter_value);

    // // printf("Matrix of inter value:\n");
    // // print_float_matrix(nb_lines, inter_value);

    // int nb_cases = 0;
    // int res = 0;
    // for (int neuron=0; neuron<32; neuron++)
    // {
    //     for (int ligne=0; ligne<26; ligne++)
    //     {
    //         for (int colonne=0; colonne<26; colonne++)
    //         {
    //         printf("Result neuron %d, case %d : %.20f\n", neuron, ligne+colonne, Conv2D_1_datas[neuron][ligne][colonne]);
    //         nb_cases++;
    //         }
    //     }
    // }
    // printf("Results : %d/%d\n", res, nb_cases);
        

    // // Check();

    // fclose(inter_file);
}

void MaxPooling2D(Couche_t* couche_in, Couche_t* couche_out)
{
    for (int neuron = 0; neuron<couche_out->nb_neurons; neuron++)
    {
        for (int line=0; line<couche_out->lines; line++)
        {
            for (int column=0; column<couche_out->columns; column++)
            {
                double max = couche_in->data[neuron][line*couche_out->kernel[0]][column*couche_out->kernel[1]];
                for (int window_x=0; window_x<couche_out->kernel[0]; window_x++)
                {
                    for (int window_y=0; window_y<couche_out->kernel[1]; window_y++)
                    {
                        if (window_x==0 && window_y==0)
                        {
                            max = couche_in->data[neuron][(line*couche_out->kernel[0])][(column*couche_out->kernel[1])];
                        }else if (couche_in->data[neuron][(line*couche_out->kernel[0])+window_x][(column*couche_out->kernel[1])+window_y] > max)
                        {
                            max = couche_in->data[neuron][(line*couche_out->kernel[0])+window_x][(column*couche_out->kernel[1])+window_y];
                        }
                    }
                }
                couche_out->data[neuron][line/couche_out->kernel[0]][column/couche_out->kernel[1]] = max;
            }
        }
    }
}

void Conv2D_2(double*** Max_Pool_datas, Conv2D_t* Conv2D_shape, Couche_t* couche, double*** Conv2D_1_datas) {
    //Pour chaque neurone à traiter
    for (int neuron = 0; neuron<Conv2D_shape->nb; neuron++)
    {
        // Pour chaque ligne de la donnée d'entrée
        for (int line = 0; line<13; line++)
        {
            // Et pour chaque colonne de la données d'entrée
            for (int column = 0; column<13; column++)
            {
                Conv2D_1_datas[neuron][line][column] = 0;
                for (int window_x = 0; window_x<Conv2D_shape->kernel[0]; window_x++)
                {
                    for (int window_y = 0; window_y<Conv2D_shape->kernel[1]; window_y++)
                    {
                        Conv2D_1_datas[neuron][line][column] += (Max_Pool_datas[neuron][line+window_x][column+window_y])*(couche->weights[neuron+window_x+window_y]);
                    }
                }
                (Conv2D_1_datas[neuron][line][column] += couche->bias[neuron]);
                // Fonction d'activation RELU
                if (Conv2D_1_datas[neuron][line][column]< 0)
                {
                    Conv2D_1_datas[neuron][line][column] = 0;
                }
            }
        }
    }
}
