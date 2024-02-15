/*******************************************************
Nom ......... : main.c
Role ........ : Permet l'execution du programme de
                traitement d'images et/ou de la prise
                de photo
Auteur ...... : Frédéric CHATRIE
Version ..... : V1.1 du 1/2/2021
Licence ..... : /

Compilation :
make veryclean
make
Pour exécuter, tapez :
./all traitement
./all photo
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bmp2Matrix.h"
#include "TraitementImage.h"

int main(int argc, const char * argv[]) {
       BMP bitmap;
       char command[200];
       char nomFichierSource[200];
       char nomFichierCible[200];
       int chiffreDebut=0;
       int chiffreFin=9;
       int quantiteDebut=0;
       int quantiteFin=20;
       unsigned char valeurSeuil=100;
       int maxOuMin=1;
       int masque=0;
       int tailleLargeurPhoto=64; // La photo doit être composé d'un minimum de 38x38 pixels
       int tailleHauteurPhoto=64;
       int tailleLargeurImageTraite=28;
       int tailleHauteurImageTraite=28;
       char buffer[200];
       strcpy(nomFichierSource,"");
       strcpy(nomFichierCible,"");
       strcpy(nomFichierSource,"../../Database/Prof/Images/bmp/");
       strcpy(nomFichierCible,"../../Database/Prof/Images/bmpProcessed/");

       if (argc>3) {
         printf("Probleme dans le nombre d'arguments\n");
       }else{
         if ((argc==2 && strcmp(argv[1],"photo")==0) || (argc==3 && (strcmp(argv[1],"photo")==0 || strcmp(argv[2],"photo")==0))){
           strcpy(command,"");
           strcpy(buffer,"");
           strcpy(command,"raspistill -e bmp -o camTest.bmp -w ");
           sprintf(buffer, "%d", tailleLargeurPhoto);
           strcat(command,buffer);
           strcat(command," -h ");
           sprintf(buffer, "%d", tailleHauteurPhoto);
           strcat(command,buffer);
           system(command);
        }
        if ((argc==2 && strcmp(argv[1],"traitement")==0) || (argc==3 && (strcmp(argv[1],"traitement")==0 || strcmp(argv[2],"traitement")==0))){
           TraitementDonnees(&bitmap,nomFichierSource,nomFichierCible, chiffreDebut,chiffreFin,quantiteDebut,quantiteFin,valeurSeuil,maxOuMin,masque,tailleLargeurImageTraite,tailleHauteurImageTraite);
        }
       }



       return 0;
}
