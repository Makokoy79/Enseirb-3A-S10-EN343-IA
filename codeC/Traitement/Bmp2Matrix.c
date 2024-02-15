/*******************************************************
Nom ......... : Bmp2Matrix.c
Role ........ : librairie permettant la lecture,
                l'écriture et l'affichage des bitmaps
Auteur ...... : Frédéric CHATRIE
Version ..... : V1.1 du 1/2/2021
Licence ..... : /

Compilation : /
Pour exécuter, tapez : /
********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "Bmp2Matrix.h"

// Fonction permettant de convertir une image lue en RBG vers un signal en niveau de gris accessible depuis la structure BMP
void ConvertRGB2Gray(BMP* pBitmap){
  int i,j;
  for (i=0;i<pBitmap->infoHeader.hauteur;i++) {
    for (j=0;j<pBitmap->infoHeader.largeur;j++) {
      pBitmap->mPixelsGray[i][j]=(pBitmap->mPixels[i][j].rouge+pBitmap->mPixels[i][j].vert+pBitmap->mPixels[i][j].bleu)/3;
    }
  }
}

// Fonction permettant l'allocation de l'espace mémoire nécessaire dans une structure à la fois pour le signal RGB et en niveau de gris
void AllouerBMP(BMP* pBitmap){
  int i;
  pBitmap->mPixels=(Pixels**) malloc(pBitmap->infoHeader.hauteur * sizeof(Pixels*));  //Allocation memoire des lignes de la matrice
  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    pBitmap->mPixels[i]=(Pixels*) malloc(pBitmap->infoHeader.largeur * sizeof(Pixels));   //Allocation memoire des colonnes de la matrice

  pBitmap->mPixelsGray=(unsigned char**) malloc(pBitmap->infoHeader.hauteur * sizeof(unsigned char*));  //Allocation memoire des lignes de la matrice
  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    pBitmap->mPixelsGray[i]=(unsigned char*) malloc(pBitmap->infoHeader.largeur * sizeof(unsigned char));   //Allocation memoire des colonnes de la matrice

}

// Fonction permettant de desallouer l'espace mémoire nécessaire à la fois pour le signal RGB et en niveau de gris
void DesallouerBMP(BMP* pBitmap){
  int i;

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixels[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixels);  //Desallocation memoire des lignes de la matrice

  for (i=0;i<pBitmap->infoHeader.hauteur;i++)
    free(pBitmap->mPixelsGray[i]);   //Desallocation memoire des colonnes de la matrice

  free(pBitmap->mPixelsGray);  //Desallocation memoire des lignes de la matrice

}

// Fonction permettant la lecture d'une image bitmap. Elle vient remplir la structure de données avec l'en-tête, l'info et la matrice de pixels
void LireBitmap(FILE* pFichier,BMP* pBitmap){
   long curseur;
   curseur=LireHeaderBitmap(pFichier,pBitmap);
   LireInfoHeaderBitmap(pFichier,pBitmap,curseur);
   AllouerBMP(pBitmap);
   LirePixelsBitmap(pFichier,pBitmap);
}

// Fonction permettant de lire l'en-tête du bitmap et qui vient stocker les valeurs dans la structure
long LireHeaderBitmap(FILE* pFichier,BMP* pBitmap){
  long curseur;
  fseek(pFichier, 0, SEEK_SET);       //place le curseur au debut du fichier
  fread(pBitmap->header.signature, 1, 2, pFichier); //lit et place la signature dans la structure
  fread(&(pBitmap->header.tailleFichier), 1, 4, pFichier); //lit et place la taille du fichier dans la structure
  fread(&(pBitmap->header.reserved), 1, 4, pFichier);
  fread(&(pBitmap->header.offset), 1, 4, pFichier);
  curseur=ftell(pFichier);
  return curseur;
}

// Fonction permettant de lire les informations du bitmap et qui vient stocker les valeurs dans la structure
void LireInfoHeaderBitmap(FILE* pFichier,BMP* pBitmap, long curseur){
  fseek(pFichier, curseur, SEEK_SET);       //place le curseur au bon endroit du fichier
  fread(&(pBitmap->infoHeader.tailleHeader), 1, 4, pFichier);    //lit et place la taille de l'en-tete dans la structure
  fread(&(pBitmap->infoHeader.largeur), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.hauteur), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.trames), 1, 2, pFichier);
  fread(&(pBitmap->infoHeader.bits), 1, 2, pFichier);
  fread(&(pBitmap->infoHeader.compression), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.tailleImage), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.xResolution), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.yResolution), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.nbCouleurs), 1, 4, pFichier);
  fread(&(pBitmap->infoHeader.couleursImportantes), 1, 4, pFichier);

}

// Fonction permettant de lire et stocker la matrice de pixels dans la structure
void LirePixelsBitmap(FILE* pFichier,BMP* pBitmap){
  int i,j,padding;
  size_t verif;
  long cur,end;

  fseek(pFichier, pBitmap->header.offset, SEEK_SET);       //place le curseur au bon endroit du fichier
  padding=4-((pBitmap->infoHeader.largeur*3)%4);  //Calcul du padding si le nombre d'octets n'est pas multiple de 4 nécessaire au format bitmap
  if (padding==4)
      padding=0;

  //Attention, dans un bitmap le bleu et rouge sont inversés
  //De plus, la lecture des pixels sur l'axe des ordonnées matriciel est différent de celui des coordonnées cartésiennes
  for (i=pBitmap->infoHeader.hauteur-1;i>=0;i--) {
    for (j=0;j<pBitmap->infoHeader.largeur;j++) {
      verif = fread(&(pBitmap->mPixels[i][j].bleu), 1, 1, pFichier);
      if (verif!=1)
        printf("Probleme de lecture pixel bleu\n");
      verif = fread(&(pBitmap->mPixels[i][j].vert), 1, 1, pFichier);
      if (verif!=1)
        printf("Probleme de lecture pixel vert\n");
      verif = fread(&(pBitmap->mPixels[i][j].rouge), 1, 1, pFichier);    //lit et place les pixels rouges dans la structure
      if (verif!=1)
        printf("Probleme de lecture pixel rouge\n");
    }
    fseek(pFichier, padding, SEEK_CUR);
  }

cur=ftell(pFichier);
fseek(pFichier, 0, SEEK_END);
end=ftell(pFichier);

if (cur==end)
  printf("Le fichier est bien termine\n");

}

// Fonction permettant l'écriture d'une image bitmap. Elle vient écrire le fichier à partir de la structure de données avec l'en-tête, l'info et la matrice de pixels
void EcrireBitmap(FILE* pFichier,BMP* pBitmap){
       long curseur;
       curseur=EcrireHeaderBitmap(pFichier,pBitmap);
       EcrireInfoHeaderBitmap(pFichier,pBitmap,curseur);
       EcrirePixelsBitmap(pFichier,pBitmap);
}

// Fonction permettant d'écrire l'en-tête du bitmap à partir des valeurs stockées dans la structure
long EcrireHeaderBitmap(FILE* pFichier,BMP* pBitmap){
  long curseur;
  fseek(pFichier, 0, SEEK_SET);       //place le curseur au debut du fichier
  fwrite(pBitmap->header.signature, 1, 2, pFichier); //ecrit la signature dans le fichier
  fwrite(&(pBitmap->header.tailleFichier), 1, 4, pFichier); //ecrit la taille du fichier dans le fichier
  fwrite(&(pBitmap->header.reserved), 1, 4, pFichier);
  fwrite(&(pBitmap->header.offset), 1, 4, pFichier);
  curseur=ftell(pFichier);
  return curseur;
}

// Fonction permettant d'écrire les informations du bitmap à partir des valeurs stockées dans la structure
void EcrireInfoHeaderBitmap(FILE* pFichier,BMP* pBitmap,long curseur){
  fseek(pFichier,curseur, SEEK_SET);       //place le curseur au bon endroit du fichier
  fwrite(&(pBitmap->infoHeader.tailleHeader), 1, 4, pFichier);    //ecrit la taille de l'en-tete dans le fichier
  fwrite(&(pBitmap->infoHeader.largeur), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.hauteur), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.trames), 1, 2, pFichier);
  fwrite(&(pBitmap->infoHeader.bits), 1, 2, pFichier);
  fwrite(&(pBitmap->infoHeader.compression), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.tailleImage), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.xResolution), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.yResolution), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.nbCouleurs), 1, 4, pFichier);
  fwrite(&(pBitmap->infoHeader.couleursImportantes), 1, 4, pFichier);

}

// Fonction permettant d'écrire la matrice de pixels sur le fichier à partir des valeurs stockées dans la structure
void EcrirePixelsBitmap(FILE* pFichier,BMP* pBitmap){
  int i,j,padding;
  size_t verif;
  unsigned char padded=0;

  fseek(pFichier, pBitmap->header.offset, SEEK_SET);       //place le curseur au bon endroit du fichier
  padding=4-((pBitmap->infoHeader.largeur*3)%4);  //Calcul du padding si le nombre d'octets n'est pas multiple de 4 nécessaire au format bitmap
  if (padding==4)
      padding=0;

  //Attention, dans un bitmap le bleu et rouge sont inversés
  //De plus, la lecture des pixels sur l'axe des ordonnées matriciel est différent de celui des coordonnées cartésiennes
  for (i=pBitmap->infoHeader.hauteur-1;i>=0;i--) {
    for (j=0;j<pBitmap->infoHeader.largeur;j++) {
      verif = fwrite(&(pBitmap->mPixels[i][j].bleu), 1, 1, pFichier);
      if (verif!=1)
        printf("Probleme d'ecriture pixel bleu\n");
      verif = fwrite(&(pBitmap->mPixels[i][j].vert), 1, 1, pFichier);
      if (verif!=1)
        printf("Probleme d'ecriture pixel vert\n");
      verif = fwrite(&(pBitmap->mPixels[i][j].rouge), 1, 1, pFichier);    //ecrit les pixels rouges dans le fichier
      if (verif!=1)
        printf("Probleme d'ecriture pixel rouge\n");
    }
    if (padding!=0) {
      verif = fwrite(&padded, padding, 1, pFichier);
      if (verif!=1)
        printf("Probleme d'ecriture padding\n");
    }
  }
}


// Fonction permettant l'affichage d'une image bitmap. Elle vient afficher les valeurs à partir de la structure de données avec l'en-tête, l'info et la matrice de pixels
void AfficherBitmap(BMP bitmap){
  int i;
  for (i=0;i<3;i++)
    printf("#######################################################\n");

  printf("############### Affichage du bitmap #################\n");

  for (i=0;i<3;i++)
    printf("#######################################################\n");

  AfficherHeaderBitmap(bitmap);
  AfficherInfoHeaderBitmap(bitmap);
  AfficherPixelsBitmap(bitmap);
}

// Fonction permettant l'affichage d'une image bitmap en niveau de gris. Elle vient afficher les valeurs à partir de la structure de données avec l'en-tête, l'info et la matrice de pixels
void AfficherBitmapGray(BMP bitmap){
  int i;
  for (i=0;i<3;i++)
    printf("#######################################################\n");

  printf("############# Affichage du bitmap en gris ###############\n");

  for (i=0;i<3;i++)
    printf("#######################################################\n");

  AfficherHeaderBitmap(bitmap);
  AfficherInfoHeaderBitmap(bitmap);
  AfficherPixelsBitmapGray(bitmap);
}

// Fonction permettant d'afficher l'en-tête du bitmap à partir des valeurs stockées dans la structure
void AfficherHeaderBitmap(BMP bitmap){
  int i;
  for (i=0;i<2;i++)
    printf("******************************************************\n");

  printf("*************** Affichage du Header ***************\n");

  for (i=0;i<2;i++)
    printf("******************************************************\n");

  printf("Signature : %s\n",bitmap.header.signature); //Affiche la signature dans la console
  printf("Taille du fichier : %d\n",bitmap.header.tailleFichier); //Affiche la taille du fichier dans la console
  printf("Taille de l'offset : %d\n",bitmap.header.offset);
}

// Fonction permettant d'afficher les informations du bitmap à partir des valeurs stockées dans la structure
void AfficherInfoHeaderBitmap(BMP bitmap){
  int i;
  for (i=0;i<2;i++)
    printf("******************************************************\n");

  printf("*************** Affichage Info Header ***************\n");

  for (i=0;i<2;i++)
    printf("******************************************************\n");

  printf("Taille du Header : %d\n",bitmap.infoHeader.tailleHeader);    //Affiche la taille de l'en-tete dans la console
  printf("Nombre de pixels sur la largeur de l'image : %d\n",bitmap.infoHeader.largeur);
  printf("Nombre de pixels sur la hauteur de l'image : %d\n",bitmap.infoHeader.hauteur);
  printf("Nombre de trames sur l'image : %d\n",bitmap.infoHeader.trames);
  printf("Nombre de bits par pixel : %d\n",bitmap.infoHeader.bits);
  printf("Compression : %d\n",bitmap.infoHeader.compression);
  printf("Taille de l'image en octets : %d\n",bitmap.infoHeader.tailleImage);
  printf("Resolution sur l'axe des X : %d\n",bitmap.infoHeader.xResolution);
  printf("Resolution sur l'axe des Y : %d\n",bitmap.infoHeader.yResolution);
  printf("Nombre de couleurs : %d\n",bitmap.infoHeader.nbCouleurs);
  printf("Couleurs importantes : %d\n",bitmap.infoHeader.couleursImportantes);

}

// Fonction permettant d'afficher la matrice de pixels à partir des valeurs stockées dans la structure
void AfficherPixelsBitmap(BMP bitmap){

  SDL_Surface* ecran = NULL;
  SDL_Surface*** matrice;
  SDL_Rect position;
  SDL_Event event;
  int i,j;
  int continuer=1;

  matrice=(SDL_Surface***) malloc(bitmap.infoHeader.hauteur * sizeof(SDL_Surface**));  //Allocation memoire des lignes de la matrice
  for (i=0;i<bitmap.infoHeader.hauteur;i++)
    matrice[i]=(SDL_Surface**) malloc(bitmap.infoHeader.largeur * sizeof(SDL_Surface*));   //Allocation memoire des colonnes de la matrice


  SDL_Init(SDL_INIT_VIDEO);

  ecran = SDL_SetVideoMode(bitmap.infoHeader.largeur, bitmap.infoHeader.hauteur, 32, SDL_HWSURFACE);

  for (i = 0 ; i < bitmap.infoHeader.hauteur ; i++){
    for (j = 0 ; j < bitmap.infoHeader.largeur ; j++)
      matrice[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);

  }

  SDL_WM_SetCaption("Affichage Bitmap", NULL);

  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

  for (i = 0 ; i < bitmap.infoHeader.hauteur ; i++){
    for (j = 0 ; j < bitmap.infoHeader.largeur ; j++){
      position.x = j; // Les lignes sont à gauche (abscisse de 0)
      position.y = (bitmap.infoHeader.hauteur-1)-i; // La position verticale dépend du numéro de la ligne
      SDL_FillRect(matrice[i][j], NULL, SDL_MapRGB(ecran->format, bitmap.mPixels[i][j].rouge, bitmap.mPixels[i][j].vert, bitmap.mPixels[i][j].bleu));
      SDL_BlitSurface(matrice[i][j], NULL, ecran, &position);
    }
  }

  SDL_Flip(ecran);

  while (continuer) // TANT QUE la variable ne vaut pas 0 //
  {
      SDL_WaitEvent(&event); // On attend un événement qu'on récupère dans event //
      switch(event.type) // On teste le type d'événement //
      {
          case SDL_QUIT: // Si c'est un événement QUITTER //
              continuer = 0; // On met le booléen à 0, donc la boucle va s'arrêter //
              break;
      }
  }

  SDL_Quit();

  for (i = 0 ; i < bitmap.infoHeader.hauteur ; i++){
    for (j = 0 ; j < bitmap.infoHeader.largeur ; j++)
      SDL_FreeSurface(matrice[i][j]);
  }

  for (i=0;i<bitmap.infoHeader.hauteur;i++)
    free(matrice[i]);   //Desallocation memoire des colonnes de la matrice

  free(matrice);  //Desallocation memoire des lignes de la matrice

  SDL_Quit();
}

// Fonction permettant d'afficher la matrice de pixels en niveau de gris à partir des valeurs stockées dans la structure
void AfficherPixelsBitmapGray(BMP bitmap){

  SDL_Surface* ecran = NULL;
  SDL_Surface*** matrice;
  SDL_Rect position;
  SDL_Event event;
  int i,j;
  int continuer=1;

  matrice=(SDL_Surface***) malloc(bitmap.infoHeader.hauteur * sizeof(SDL_Surface**));  //Allocation memoire des lignes de la matrice
  for (i=0;i<bitmap.infoHeader.hauteur;i++)
    matrice[i]=(SDL_Surface**) malloc(bitmap.infoHeader.largeur * sizeof(SDL_Surface*));   //Allocation memoire des colonnes de la matrice


  SDL_Init(SDL_INIT_VIDEO);

  ecran = SDL_SetVideoMode(bitmap.infoHeader.largeur, bitmap.infoHeader.hauteur, 32, SDL_HWSURFACE);

  for (i = 0 ; i < bitmap.infoHeader.hauteur ; i++){
    for (j = 0 ; j < bitmap.infoHeader.largeur ; j++)
      matrice[i][j] = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);

  }

  SDL_WM_SetCaption("Affichage Bitmap", NULL);

  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

  for (i = 0 ; i < bitmap.infoHeader.hauteur ; i++){
    for (j = 0 ; j < bitmap.infoHeader.largeur ; j++){
      position.x = j; // Les lignes sont à gauche (abscisse de 0)
      position.y = (bitmap.infoHeader.hauteur-1)-i; // La position verticale dépend du numéro de la ligne
      SDL_FillRect(matrice[i][j], NULL, SDL_MapRGB(ecran->format, bitmap.mPixelsGray[i][j], bitmap.mPixelsGray[i][j], bitmap.mPixelsGray[i][j]));
      SDL_BlitSurface(matrice[i][j], NULL, ecran, &position);
    }
  }

  SDL_Flip(ecran);

  while (continuer) // TANT QUE la variable ne vaut pas 0 //
  {
      SDL_WaitEvent(&event); // On attend un événement qu'on récupère dans event //
      switch(event.type) // On teste le type d'événement //
      {
          case SDL_QUIT: // Si c'est un événement QUITTER //
              continuer = 0; // On met le booléen à 0, donc la boucle va s'arrêter //
              break;
      }
  }

  SDL_Quit();

  for (i = 0 ; i < bitmap.infoHeader.hauteur ; i++){
    for (j = 0 ; j < bitmap.infoHeader.largeur ; j++)
      SDL_FreeSurface(matrice[i][j]);
  }

  for (i=0;i<bitmap.infoHeader.hauteur;i++)
    free(matrice[i]);   //Desallocation memoire des colonnes de la matrice

  free(matrice);  //Desallocation memoire des lignes de la matrice

  SDL_Quit();
}
