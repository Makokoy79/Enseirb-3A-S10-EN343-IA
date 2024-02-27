#include <stdio.h>
#include <stdlib.h>

int main() {
    if (system("./Traitement/all photo traitement") != 0) {
        fprintf(stderr, "Erreur: Veuillez compiler le projet Traitement d'abord.\n");
        return 1;
    }
    if (system("./Inference/all camTest.bmp") != 0) {
        fprintf(stderr, "Erreur: Veuillez compiler le projet Inférence d'abord.\n");
        return 1;
    }
    return 0;
}