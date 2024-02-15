/*******************************************************
Nom ......... : TraitementImage.h
Role ........ : Permet le traitement des images bitmaps
Auteur ...... : Frédéric CHATRIE
Version ..... : V1.1 du 1/2/2021
Licence ..... : /

Compilation : /
Pour exécuter, tapez : /
********************************************************/

#ifndef TRAITEMENTIMAGE_H
#define TRAITEMENTIMAGE_H

#include "Bmp2Matrix.h"

//Prototypes des fonctions
void RognerBitmapMax(BMP* pBitmap,unsigned char valMax);
void RognerBitmapMin(BMP* pBitmap,  unsigned char valMin);
void TraitementDonnees(BMP* pBitmap, char* nomFichierSource, char* nomFichierCible, int chiffreDebut, int chiffreFin, int quantiteDebut,int quantiteFin,unsigned char valeurSeuil, int maxOuMin, int masque,int tailleLargeurImageTraite,int tailleHauteurImageTraite);
#endif
