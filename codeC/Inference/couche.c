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

#include <math.h>

void pause()
{
    char chaine[2];
    printf("Appuyez sur Entrée pour continuer\n");
    fgets(chaine, 2, stdin);
}

int is_file_auth(char *argv1)
{
    int nb_of_valid_files = 11;
    char *files[] = {"0_20.bmp", "1_20.bmp", "2_20.bmp", "3_20.bmp", "4_20.bmp", "5_20.bmp", "6_20.bmp", "7_20.bmp", "8_20.bmp", "9_20.bmp", "camTest.bmp"};

    for(int index=0; index<nb_of_valid_files; index++)
    {
        if (!strcmp(files[index], argv1))
        {
            return 1;
        }
    }
    return 0;
}
  
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

/* 
void read_file(FILE *file, int nb_lines, int nb_values_per_line, double* texte) {
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    char buffer[1000000];

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
 */

void read_file(FILE *file, int nb_lines, int nb_values_per_line, double* texte) {
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    // Allouer la mémoire dynamiquement pour le buffer
    int taille_buffer = 50000;
    char *buffer = (char *)malloc(taille_buffer * sizeof(char));
    if (buffer == NULL) {
        perror("Erreur d'allocation de mémoire");
        return;
    }

    for (int i = 0; i < nb_lines; i++) {
        if (fgets(buffer, taille_buffer, file) != NULL) {
            // Lire les valeurs dans la mémoire allouée
            char *token = strtok(buffer, " ");
            int j = 0;
            while (token != NULL && j < nb_values_per_line) {
                texte[i * nb_values_per_line + j] = atof(token);
                token = strtok(NULL, " ");
                j++;
            }
        } else {
            printf("Erreur de lecture de la ligne %d\n", i + 1);
            free(buffer); // Libérer la mémoire avant de quitter en cas d'erreur
            exit(EXIT_FAILURE);
        }
    }

    free(buffer); // Libérer la mémoire une fois que nous avons terminé avec le buffer
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
        //couche->nb_neurons = 0;
        couche->nb_weights = 0;
        couche->nb_bias = 0;
        return;
    }

    nb_lines = calcul_nb_line(weights_file);

    couche->nb_weights = calcul_nb_values_per_line(weights_file);
    printf("Number of weights per neuron %d\n", couche->nb_weights);

    if (nb_lines == 0) {
        printf("No neurons in layer %d\n", i);
        //couche->nb_neurons = 0;
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

// Fonction d'allocation de la mémoire pour chaque couche
void Alloc_memory_datas(Model_t *Neural_net)
{
  for (int layer=0; layer<Neural_net->nb_couche; layer++)
  {
  Neural_net->couches[layer].data = (double***)malloc(Neural_net->couches[layer].nb_neurons * sizeof(double**));
  for (int i = 0; i < Neural_net->couches[layer].nb_neurons; ++i)
    {
      Neural_net->couches[layer].data[i] = (double**)malloc(Neural_net->couches[layer].lines * sizeof(double*));
      for (int j = 0; j < Neural_net->couches[layer].lines; ++j)
      {
          Neural_net->couches[layer].data[i][j] = (double*)malloc(Neural_net->couches[layer].columns * sizeof(double));
      }
    }
  }
}

// Fonction de libération de la mémoire pour chaque couche
void Free_memory_datas(Model_t *Neural_net)
{
  for (int layer=0; layer<Neural_net->nb_couche; layer++) 
  {
    free(Neural_net->couches[layer].data);
  }
}

void Conv2D(Couche_t* couche_in, Couche_t* couche_out) {
    int kernel_size = couche_out->kernel[0]*couche_out->kernel[1];
    //Pour chaque neurone à traiter
    for (int neuron = 0; neuron<couche_out->nb_neurons; neuron++)
    {
        //pause();    // faire un pause en débug. A commenter en opérationnel

        // Pour chaque ligne de la donnée de sortie
        for (int line = 0; line<(couche_out->lines); line++)
        {
            // Et pour chaque colonne de la données de sortie
            for (int column = 0; column<(couche_out->columns); column++)
            {
                //printf("ligne %d Colonne %d\n", line, column);
                // Pour chaque neurone d'entrée
                double value_out = 0;
                for (int neuron_in=0; neuron_in<couche_in->nb_neurons; neuron_in++)
                {
                    // double value_out = 0;
                    // Pour chaque élément de la fenêtre de convolution (Kernel)
                    // printf("Neurone out %d neurone in %d\n", neuron, neuron_in);
                    
                    for (int window_x = 0; window_x<couche_out->kernel[0]; window_x++)
                    {
                        for (int window_y = 0; window_y<couche_out->kernel[1]; window_y++)
                        {
                            // Cherche le bon poids à appliquer
                            double weight = couche_out->weights[neuron*couche_in->nb_neurons*kernel_size + neuron_in*kernel_size + window_x*couche_out->kernel[1]+window_y];
                            // Appliquer le poids 
                            value_out += (couche_in->data[neuron_in][line+window_x][column+window_y])*weight;
/*                             printf("poids : %.20f\t", weight);
                            // Affiche les valeurs de la donnée d'entrée
                            printf("%f\t", couche_in->data[neuron_in][line+window_x][column+window_y]);
                            pause();
                            */
                        }
                        // printf("\n");
                    }
                    // printf("\n");
                }
                // Application du biais
                // vérification du bon biais appliqué (à commenter en opérationnel)
                //printf("Biais appliqué neurone %d : %f\n", neuron, couche_out->bias[neuron]);
                value_out += couche_out->bias[neuron];
                // Fonction d'activation RELU
                if (value_out< 0)
                {
                    value_out = 0;
                }
                couche_out->data[neuron][line][column] = value_out;
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
                double max = 0;
                for (int window_x=0; window_x<couche_out->kernel[0]; window_x++)
                {
                    int index_x = line * couche_out->kernel[0] + window_x;
                    for (int window_y=0; window_y<couche_out->kernel[1]; window_y++)
                    {
                        int index_y = column * couche_out->kernel[1] + window_y;
                        if (window_x==0 && window_y==0)
                        {
                            max = couche_in->data[neuron][index_x][index_y];
                        }else if (couche_in->data[neuron][index_x][index_y] > max)
                        {
                            max = couche_in->data[neuron][index_x][index_y];
                        }
                    }
                }
                couche_out->data[neuron][line][column] = max;
            }
        }
    }
}

void flatten(Couche_t* couche_in, Couche_t* couche_out)
{
    int pixel=0;
    for (int column=0; column<couche_in->columns; column++)
    {
        for (int line=0; line<couche_in->lines; line++)
        {
            for (int neuron=0; neuron<couche_in->nb_neurons; neuron++)
            {
                couche_out->data[0][0][pixel] = couche_in->data[neuron][line][column];
                pixel++;
            }
        }
    }
}

void dense(Couche_t* couche_in, Couche_t* couche_out)
{
    double somme = 0.0;
    // printf("Nombre de classes : %d\n", couche_out->columns);
    for (int classe=0; classe<couche_out->columns; classe++)
    {
        couche_out->data[0][0][classe] = 0;
        for (int entree=0; entree<couche_in->columns; entree++)
        {
            couche_out->data[0][0][classe] += couche_in->data[0][0][entree] * couche_out->weights[classe*couche_in->columns+entree];
/* 
            //printf("Poids classe %d neurone_in %d : %f\n", classe, entree, couche_out->weights[classe*couche_in->columns+entree]);
            if (classe==0)
            {
                printf("%f\t", couche_in->data[0][0][entree]);
            }
*/
        }
        couche_out->data[0][0][classe] += couche_out->bias[classe];
        couche_out->data[0][0][classe] = exp(couche_out->data[0][0][classe]);
        somme += couche_out->data[0][0][classe];        
    }
    // Application de la fonction softmax
    for (int classe = 0; classe < couche_out->columns; classe++)
    {
        // Application de softmax à chaque score
        couche_out->data[0][0][classe] /= somme;
        //printf("Résultat classe %d : %f\n", classe, couche_out->data[0][0][classe]);
    }
}