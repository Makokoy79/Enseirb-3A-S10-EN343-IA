/*******************************************************
Nom ......... : main.c
Role ........ : Programme principal executant :
                    La prise d'une photo
                    Le traitement d'une photo
                    La lecture et l'inférence
                    Donne le résultat de la prédiction
                        avec les scores liés

Auteurs ..... : Lilian BRUNET et Rémy CROUZAT
Version ..... : V1.0 du 27/02/2024
Licence ..... : /

Compilation :
make veryclean
make
Pour exécuter, tapez : ./all
********************************************************/
/*

#include <stdio.h>
#include <stdlib.h>

int main() {
    if (system("./Traitement/all photo traitement") != 0) {
        fprintf(stderr, "Erreur d'exécution\n Veuillez vérifier que le projet Traitement est bien compilé.\n");
        return 1;
    }
    if (system("./Inference/all camTest.bmp") != 0) {
        fprintf(stderr, "Erreur d'exécution\n Veuillez vérifier que le projet Traitement est bien compilé.\n");
        return 1;
    }
    return 0;
}*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int choix, couche;
    char commande[256], nomFichier[256], coucheStr[10];
    char argumentPhoto[10], argumentTraitement[12], afficherResultats[2];

    printf("Menu :\n");
    printf("1. Traitement\n");
    printf("2. Inférence\n");
    printf("3. Tout\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1: // Traitement
            printf("Ajouter l'argument 'photo' ? (oui=1/non=0) : ");
            scanf("%s", argumentPhoto);
            printf("Ajouter l'argument 'traitement' ? (oui=1/non=0) : ");
            scanf("%s", argumentTraitement);

            strcpy(commande, "./Traitement/all");
            if (strcmp(argumentPhoto, "1") == 0) {
                strcat(commande, " photo");
            }
            if (strcmp(argumentTraitement, "1") == 0) {
                strcat(commande, " traitement");
            }
            break;
        case 2: // Inférence
            printf("Spécifier un nom de fichier ? (oui=1/non=0) : \n");
            printf("\tSi non le fichier par défaut sera 0_20.bmp => ");
            scanf("%s", afficherResultats);
            if (strcmp(afficherResultats, "1") == 0) {
                printf("Nom du fichier : ");
                scanf("%s", nomFichier);
            } else {
                strcpy(nomFichier, "0_20.bmp");
            }

            printf("Afficher les résultats intermédiaires ? (oui=1/non=0) : ");
            scanf("%s", afficherResultats);
            if (strcmp(afficherResultats, "1") == 0) {
                printf("Quelle couche (0 à 5) ?\n");
                printf("\t0 : Affiche les valeurs de pixel de l'image copiées dans le réseau de neurone\n");
                printf("\t1 : Affiche les valeurs intermédiaires de la 1ère convolution 2D\n");
                printf("\t2 : Affiche les valeurs intermédiaires du 1er maxPooling \n");
                printf("\t3 : Affiche les valeurs intermédiaires de la 2ème convolution 2D \n");
                printf("\t4 : Affiche les valeurs de pixel du 2ème maxpooling\n");
                printf("\t5 : Affiche les valeurs de pixel de la couche flatten\n");
                scanf("%d", &couche);
                if (couche < 0 || couche > 5) {
                    printf("Erreur de saisie. Utilisation de la couche 0 par défaut.\n");
                    couche = 0;
                }
                sprintf(coucheStr, "couche%d", couche);
                strcpy(commande, "./Inference/all ");
                strcat(commande, nomFichier);
                strcat(commande, " ");
                strcat(commande, coucheStr);
            } else {
                strcpy(commande, "./Inference/all ");
                strcat(commande, nomFichier);
            }
            break;
        case 3: // Tout
            strcpy(commande, "./Traitement/all photo traitement && ./Inference/all 0_20.bmp");
            break;
        default:
            printf("Choix invalide.\n");
            return 1;
    }

    printf("Exécution de la commande : %s\n", commande);
    
    system(commande);

    return 0;
}
