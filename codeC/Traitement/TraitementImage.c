/*******************************************************
Nom ......... : TraitementImage.c
Role ........ : Permet le traitement des images bitmaps
Auteur ...... : Frédéric CHATRIE
Version ..... : V1.1 du 1/2/2021
Licence ..... : /

Compilation : /
Pour exécuter, tapez : /
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Bmp2Matrix.h"
#include "TraitementImage.h"

//Fonction permettant de rogner le signal de l'image selon une valeur donnée en seuil maximum
void RognerBitmapMax(BMP* pBitmap,unsigned char valMax){
  int i;
  int j=0;
  int padding;
  int margeGauche=0;
  int margeHaute=0;
  int margeDroite=pBitmap->infoHeader.largeur;
  int margeBasse=pBitmap->infoHeader.hauteur;
  int continu=1;
  int nouvelleHauteur;
  int nouvelleLargeur;


  Pixels** mPixels;
  unsigned char** mPixelsGray;

  continu=1;
  j=0;
  while(continu && j<pBitmap->infoHeader.largeur){
    for(i=0;i<pBitmap->infoHeader.hauteur;i++){
      if(pBitmap->mPixelsGray[i][j]>valMax)
        continu=0;
    }
    j++;
  }
  if(j<pBitmap->infoHeader.largeur-1)
    margeGauche=j-1;

  continu=1;
  j=pBitmap->infoHeader.largeur-1;
  while(continu && j>=0){
    for(i=0;i<pBitmap->infoHeader.hauteur;i++){
      if(pBitmap->mPixelsGray[i][j]>valMax)
        continu=0;
      }
    j--;
  }
  if(j>0)
    margeDroite=j+1;

  continu=1;
  i=0;
  while(continu && i<pBitmap->infoHeader.hauteur){
    for(j=0;j<pBitmap->infoHeader.largeur;j++){
      if(pBitmap->mPixelsGray[i][j]>valMax)
        continu=0;
      }
    i++;
  }
  if(i<pBitmap->infoHeader.hauteur-1)
    margeHaute=i-1;

  continu=1;
  i=pBitmap->infoHeader.hauteur-1;
  while(continu && i>=0){
    for(j=0;j<pBitmap->infoHeader.largeur;j++){
      if(pBitmap->mPixelsGray[i][j]>valMax)
        continu=0;
      }
    i--;
  }
  if(i>0)
    margeBasse=i+1;

  nouvelleHauteur=((pBitmap->infoHeader.hauteur-1)-margeHaute)-((pBitmap->infoHeader.hauteur-1)-margeBasse);
  nouvelleLargeur=((pBitmap->infoHeader.largeur-1)-margeGauche)-((pBitmap->infoHeader.largeur-1)-margeDroite);

  mPixels=(Pixels**) malloc(nouvelleHauteur*sizeof(Pixels*));
  for (i=0;i<nouvelleHauteur;i++)
      mPixels[i]=(Pixels*) malloc(nouvelleLargeur*sizeof(Pixels));

  mPixelsGray=(unsigned char**) malloc(nouvelleHauteur*sizeof(unsigned char*));
  for (i=0;i<nouvelleHauteur;i++)
      mPixelsGray[i]=(unsigned char*) malloc(nouvelleLargeur*sizeof(unsigned char));

  for(i=0;i<nouvelleHauteur;i++){
    for(j=0;j<nouvelleLargeur;j++){
        mPixelsGray[i][j]=pBitmap->mPixelsGray[i+margeHaute][j+margeGauche];
        mPixels[i][j].rouge=pBitmap->mPixels[i+margeHaute][j+margeGauche].rouge;
        mPixels[i][j].vert=pBitmap->mPixels[i+margeHaute][j+margeGauche].vert;
        mPixels[i][j].bleu=pBitmap->mPixels[i+margeHaute][j+margeGauche].bleu;
    }
  }

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixels[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixels);  //Desallocation memoire des lignes de la matrice

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixelsGray[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixelsGray);  //Desallocation memoire des lignes de la matrice

  pBitmap->mPixels=mPixels;
  pBitmap->mPixelsGray=mPixelsGray;
  pBitmap->infoHeader.hauteur=nouvelleHauteur;
  pBitmap->infoHeader.largeur=nouvelleLargeur;

  padding=4-((pBitmap->infoHeader.largeur*3)%4);  //Calcul du padding si le nombre d'objet n'est pas multiple de 4
  if (padding==4)
      padding=0;

  pBitmap->header.tailleFichier=pBitmap->infoHeader.hauteur*(pBitmap->infoHeader.largeur*3+padding)+pBitmap->header.offset;

}


//Fonction permettant de rogner le signal de l'image selon une valeur donnée en seuil minimum
void RognerBitmapMin(BMP* pBitmap,  unsigned char valMin){
  int i;
  int j=0;
  int padding;
  int margeGauche=0;
  int margeHaute=0;
  int margeDroite=pBitmap->infoHeader.largeur;
  int margeBasse=pBitmap->infoHeader.hauteur;
  int continu=1;
  int nouvelleHauteur;
  int nouvelleLargeur;


  Pixels** mPixels;
  unsigned char** mPixelsGray;

  continu=1;
  j=0;
  while(continu && j<pBitmap->infoHeader.largeur){
    for(i=0;i<pBitmap->infoHeader.hauteur;i++){
      if(pBitmap->mPixelsGray[i][j]<valMin)
        continu=0;
      }
    j++;
  }
  if(j<pBitmap->infoHeader.largeur-1)
    margeGauche=j-1;

  continu=1;
  j=pBitmap->infoHeader.largeur-1;
  while(continu && j>=0){
    for(i=0;i<pBitmap->infoHeader.hauteur;i++){
      if(pBitmap->mPixelsGray[i][j]<valMin)
        continu=0;
      }
    j--;
  }
  if(j>0)
    margeDroite=j+1;

  continu=1;
  i=0;
  while(continu && i<pBitmap->infoHeader.hauteur){
    for(j=0;j<pBitmap->infoHeader.largeur;j++){
      if(pBitmap->mPixelsGray[i][j]<valMin)
        continu=0;
      }
    i++;
  }
  if(i<pBitmap->infoHeader.hauteur-1)
    margeHaute=i-1;

  continu=1;
  i=pBitmap->infoHeader.hauteur-1;
  while(continu && i>=0){
    for(j=0;j<pBitmap->infoHeader.largeur;j++){
      if(pBitmap->mPixelsGray[i][j]<valMin)
        continu=0;
      }
    i--;
  }
  if(i>0)
    margeBasse=i+1;

  nouvelleHauteur=pBitmap->infoHeader.hauteur-margeHaute-((pBitmap->infoHeader.hauteur-1)-margeBasse);
  nouvelleLargeur=pBitmap->infoHeader.largeur-margeGauche-((pBitmap->infoHeader.largeur-1)-margeDroite);

  mPixels=(Pixels**) malloc(nouvelleHauteur*sizeof(Pixels*));
  for (i=0;i<nouvelleHauteur;i++)
      mPixels[i]=(Pixels*) malloc(nouvelleLargeur*sizeof(Pixels));

  mPixelsGray=(unsigned char**) malloc(nouvelleHauteur*sizeof(unsigned char*));
  for (i=0;i<nouvelleHauteur;i++)
      mPixelsGray[i]=(unsigned char*) malloc(nouvelleLargeur*sizeof(unsigned char));

  for(i=0;i<nouvelleHauteur;i++){
    for(j=0;j<nouvelleLargeur;j++){
        mPixelsGray[i][j]=pBitmap->mPixelsGray[i+margeHaute][j+margeGauche];
        mPixels[i][j].rouge=pBitmap->mPixels[i+margeHaute][j+margeGauche].rouge;
        mPixels[i][j].vert=pBitmap->mPixels[i+margeHaute][j+margeGauche].vert;
        mPixels[i][j].bleu=pBitmap->mPixels[i+margeHaute][j+margeGauche].bleu;
    }
  }

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixels[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixels);  //Desallocation memoire des lignes de la matrice

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixelsGray[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixelsGray);  //Desallocation memoire des lignes de la matrice

  pBitmap->mPixels=mPixels;
  pBitmap->mPixelsGray=mPixelsGray;
  pBitmap->infoHeader.hauteur=nouvelleHauteur;
  pBitmap->infoHeader.largeur=nouvelleLargeur;

  padding=4-((pBitmap->infoHeader.largeur*3)%4);  //Calcul du padding si le nombre d'objet n'est pas multiple de 4
  if (padding==4)
      padding=0;

  pBitmap->header.tailleFichier=pBitmap->infoHeader.hauteur*(pBitmap->infoHeader.largeur*3+padding)+pBitmap->header.offset;
}


//Fonction permettant d'appliquer un masque au signal de l'image selon une valeur seuil fournie
void MasqueBitmap(BMP* pBitmap, unsigned char val){
  int i;
  int j;

  for (i=0;i<pBitmap->infoHeader.hauteur;i++) {
    for (j=0;j<pBitmap->infoHeader.largeur;j++) {
      if(pBitmap->mPixelsGray[i][j]>val)
        pBitmap->mPixelsGray[i][j]=255;
      else
        pBitmap->mPixelsGray[i][j]=0;

      pBitmap->mPixels[i][j].rouge=pBitmap->mPixelsGray[i][j];
      pBitmap->mPixels[i][j].vert=pBitmap->mPixelsGray[i][j];
      pBitmap->mPixels[i][j].bleu=pBitmap->mPixelsGray[i][j];
    }
  }
}

//Fonction permettant d'appliquer les traitements choisis par l'utilisateur et de traiter tout une base de donnée
void TraitementDonnees(BMP* pBitmap, char* nomFichierSource, char* nomFichierCible, int chiffreDebut, int chiffreFin, int quantiteDebut,int quantiteFin,unsigned char valeurSeuil, int maxOuMin, int masque,int tailleLargeurImageTraite,int tailleHauteurImageTraite){
  FILE* pFichier;
  char command[200];
  char nomFichierSourceTmp[200];
  char nomFichierCibleTmp[200];
  char buffer[200];
  char nom[200];
  int i,j;
  for (i=chiffreDebut;i<=chiffreFin;i++) {
    for (j=quantiteDebut;j<=quantiteFin;j++) {
      strcpy(nomFichierSourceTmp,"");
      strcpy(nomFichierSourceTmp,nomFichierSource);
      strcpy(nom,"");
      sprintf(nom,"%d_%d.bmp",i,j);
      strcat(nomFichierSourceTmp,nom);
      pFichier=fopen(nomFichierSourceTmp, "rb");     //Ouverture du fichier contenant l'image
      if (pFichier==NULL) {
          printf("%s\n", nomFichierSourceTmp);
          printf("Erreur dans la lecture du fichier\n");
      }
      LireBitmap(pFichier, pBitmap);
      fclose(pFichier);               //Fermeture du fichier contenant l'image

      ConvertRGB2Gray(pBitmap);

      if (maxOuMin==1)
        RognerBitmapMax(pBitmap,valeurSeuil);
      else if (maxOuMin==2)
        RognerBitmapMin(pBitmap,valeurSeuil);
      if (masque==1)
        MasqueBitmap(pBitmap,valeurSeuil);

      strcpy(nomFichierCibleTmp,"");
      strcpy(nomFichierCibleTmp,nomFichierCible);
      strcat(nomFichierCibleTmp,nom);
      //strcpy(command,"");
      //strcpy(command,"touch ");
      //strcat(command,nomFichierCibleTmp);
      //system(command);

      pFichier=fopen(nomFichierCibleTmp, "wb");     //Ouverture du fichier contenant l'image
      if (pFichier==NULL) {
          printf("%s\n", nomFichierCibleTmp);
          printf("Erreur dans l'ecriture du fichier\n");
      }
      EcrireBitmap(pFichier, pBitmap);
      fclose(pFichier);               //Fermeture du fichier contenant l'image
      DesallouerBMP(pBitmap);
      strcpy(command,"");
      strcpy(buffer,"");
      strcpy(command,"convert ");
      strcat(command,nomFichierCibleTmp);
      strcat(command," -resize ");
      sprintf(buffer, "%d", tailleLargeurImageTraite);
      strcat(command,buffer);
      strcat(command,"x");
      sprintf(buffer, "%d", tailleHauteurImageTraite);
      strcat(command,buffer);
      strcat(command,"! -quality 100 ");
      strcat(command,nomFichierCibleTmp);
      system(command);

   }
  }
}
