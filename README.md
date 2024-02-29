# A lire pour se servir correctement du programme de reconnaissance de chiffres

## Généralités
Ce programme a été écrit par Lilian Brunet et Rémy Crouzat en février 2024 pour le cours EN343 "développement IA légère sur un système embarqué.

Sur un raspberry pi, il permet de :
* Prendre une photo avec la caméra officielle RaspberryPi,
* La traiter et la stocker
* L'ouvrir et la passer en niveaux de gris
* Réaliser l'inférence afin d'en déduire le chiffre inscrit

## Architecture
Le projet est composé de 2 dossiers principaux :
* **CodeC** : Contient les répertoires suivants :
   - **Traitement** : dossier contenant les sources (et les exécutables après compilation) permettant la prise de d'une photo et/ou d'effectuer le traitement de l'ensemble des photos d'un répertoire. En l'état actuel, le répertoire est codé "en dur" dans le programme.
   - **Inférence** : Dossier contenant les sources (et les exécutables après compilation) permettant de réaliser l'opération d'inférence et de donner le résultat de l'analyse de l'image traitée
   - **Paramètres** : Dossier contenant les fichiers de poids et de biais à utiliser lors de l'inférence. Il contient également les résultats inermédiaires issus du code python qui a généré les fichiers de poids et de biais et qui a testé sa validité sur l'image 0_20.bmp. Ces fichiers (interx.txt) permettent de vérifier les résultats intermédiaires générés par le code C et de valider le bon fonctionnement de ce dernier. 
* **Database** : Contient un ensemble d'images pouvant servir au test du programme.

A la racine se trouvent aussi le fichier main.c permettant d'afficher le menu général de l'application, et un fichier Makefile facilitant la compilation de l'ensemble des fichiers. 

## Compilation
Avant toute chose, il faut se situer à la **racine du répertoire** de projet (répertoire ./codeC/). Deux commandes principales sont à exécuter :
* pour tout supprimer d'une éventuelle précédente compilation et être sûr que nous redémarrons sur des bases saines :

       make veryclean

* pour compiler l'ensemble des exécutables nécessaires à l'exécution du projet

        make

## Exécution
Une fois la compilation réalisée avec succès, nous pouvons exécuter la commande suivante :

       ./all
Un menu à 3 choix apparait alors :

     1. Traitement
     2. Inférence
     3. Tout
     Votre choix :

### Menu 1 : Traitement
Avec le choix de ce menu, la partie inférence ne sera pas exécutée. Nous aurons alors un 1er choix :

      Ajouter l'argument 'photo' ? (oui=1/non=0) : 
Si oui, l'application prendra une photo pour éventuellement traiter un nouveau cliché.
Puis un nouveau choix est proposé à la suite :

      Ajouter l'argument 'traitement' ? (oui=1/non=0) : 
Pour réaliser le traiment du nouveau cliché, ou d'un précédent s'il existe.
Si la réponse est non aux 2 propositions, alors le programme s'arrête et rien n'est exécuté.

### Menu 2 : Inférence
Lorsque ce choix est fait, une première question est posée :

      Spécifier un nom de fichier ? (oui=1/non=0) : 
        Si non le fichier par défaut sera 0_20.bmp => 
Les fichiers valides sont nommés \<chiffre>_20.bmp, avec \<chiffre> pouvant être de 0 à 9. Si on choisi "oui" et un mauvais nom de fichier ou si on choisi "non", alors c'est la photo 0_20.bmp sur laquelle sera réalisée l'inférence.

Vient ensuite un 2ème choix : 

      Afficher les résultats intermédiaires ? (oui=1/non=0) : 
Si non, alors le programe s'exécute normalement. Si oui alors un dernier choix s'offre à nous :

        0 : Affiche les valeurs de pixel de l'image copiées dans le réseau de neurone
        1 : Affiche les valeurs intermédiaires de la 1ère convolution 2D
        2 : Affiche les valeurs intermédiaires du 1er maxPooling 
        3 : Affiche les valeurs intermédiaires de la 2ème convolution 2D 
        4 : Affiche les valeurs de pixel du 2ème maxpooling
        5 : Affiche les valeurs de pixel de la couche flatten
Un seul choix parmis les 6 est possible. Ensuite le programme d'Inférence finit par s'exécuter.

### Menu 3 : Tout

#### Principe
Avec ce dernier choix le programme va automatiquement :
* Prendre une photo
* La traiter
* Réaliser l'opération d'inférence
* Donner le résultat de prédiction avec le score associé

 #### Fonctionnement
 Dès la validation du choix, la caméra s'active et l'image apparait (si le raspberry pi est connecté à un écran bien sûr).Cela permet pendant quelques secondes de bien positionner le chiffre manuscrit au centre de l'image. Le traitement et l'inférence fonctionnent mieux si le fond est noir et le caractère clair.

 Une fois le temps écoulé la photo est prise et stockée dans le répertoire ../Database/Bruzat/bmp.

 Ensuite l'image est rognée, seuillée puis le résultat sauvé dans le répertoire ../Database/Bruzat/bmpProcessed.

 La dernière étape, l'inférence, est alors excécutée avec en paramètre le chemin vers l'image traitée.

 Comme pour le choix 2-1-0, le programme affiche le score pour chacune des classes et la classe ayant obtenu le plus grand score.

