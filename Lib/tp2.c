#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "def.h"
#include "nrio.h"
#include "nrarith.h"
#include "nralloc.h"

/**

  TP1
  Prise en main.
  gcc -o tp1 nralloc.c nrio.c nrarith.c tp1.c -W -lm


**/

/**
    Fonction qui binarise une image suivant la valeur de seuil et l'enregistre.
    Si un pixel est inférieur à seul, on le met à 0.
    Sinon on le met à 255.
**/


byte** binarisation(int seuil){
    int i, j ;
    long nrh, nrl, nch, ncl;
    byte **I, **I2 ;

    I = LoadPGM_bmatrix("rice.pgm",&nrl,&nrh,&ncl,&nch);
    I2 = bmatrix(nrl,nrh,ncl,nch);

    for(i = 0 ; i < nrh ; i++){
        for (j = 0 ; j < nch ; j++){
            int pixel = 0 ;
            if(I[i][j] < seuil){
                pixel = 0 ;
            }
            else {
                pixel = 255 ;
            }
            I2[i][j] = (byte)pixel ;
        }
    }

    SavePGM_bmatrix(I2,nrl,nrh,ncl,nch,"rice_bin.pgm");

    free_bmatrix(I,nrl,nrh,ncl,nch);
    //free_bmatrix(I2,nrl,nrh,ncl,nch);

    return I2 ;
}

/**
    Fonction qui fait l'etiquetage d'une image :
        - Elle binarise tout d'abord l'image
        - On applique ensuite l'algorithme intuitif d'étiquetage.

**/
void etiquetage(int seuil, int etiquette){
    int i, j, compteur_region, k, l ;
    long nrh, nrl, nch, ncl;
    byte **I, **I_bin, **I2, **I3 ;

    compteur_region = 1 ;
    i = j = k = l = 0 ;

    I = LoadPGM_bmatrix("rice.pgm",&nrl,&nrh,&ncl,&nch);
    I_bin = bmatrix(nrl,nrh,ncl,nch);
    I2 = bmatrix(nrl,nrh,ncl,nch);
    I3 = bmatrix(nrl,nrh,ncl,nch);


    int regions[nrh][nch];

    I_bin = binarisation(seuil);

    for (i = 1 ; i < nrh ; i++){
        for (j = 1 ; j < nch ; j++){
            if(I_bin[i][j] == 255){
                if((I_bin[i][j] == I_bin[i][j-1]) && (I_bin[i][j] != I_bin[i-1][j])){
                    regions[i][j] = regions[i][j-1] ;
                }
                else if((I_bin[i][j] == I_bin[i-1][j]) && (I_bin[i][j] != I_bin[i][j-1])){
                    regions[i][j] = regions[i-1][j];
                }
                else if ((I_bin[i][j] != I_bin[i-1][j]) && (I_bin[i][j] != I_bin[i][j-1])){
                    regions[i][j] = compteur_region ;
                    compteur_region++ ;
                }
                else if((I_bin[i][j] == I_bin[i-1][j]) && (I_bin[i][j] == I_bin[i][j-1]) && (regions[i][j-1] == regions[i-1][j])){
                    regions[i][j] = regions[i-1][j];
                }
                else {
                    regions[i][j] = regions[i-1][j];
                    int etiquette_a_changer = regions[i][j-1]; // On garde en mémoire la valeur de l'étiquette à changer.
                    for(k = 0 ; k < i ; k++){
                        for(l = 0 ; l < nrh && l < j; l++){
                            if(regions[k][l] == etiquette_a_changer){
                                regions[k][l] = regions[i][j];
                            }
                        }
                    }
                }
            }
        }
    }

    printf("\n\nNombre d'étiquettes = %d\n\n", compteur_region);

    //normalisation = pixel*255/max etiquette
    for(i = 0 ; i < nrh-1 ; i++){
        for(j = 0 ; j < nch-1 ; j++){
            regions[i][j] = (int)regions[i][j]*255/compteur_region;
            I2[i][j] = (byte)regions[i][j];
        }
    }

    SavePGM_bmatrix(I2,nrl,nrh,ncl,nch,"rice_etiq.pgm");

    free_bmatrix(I,nrl,nrh,ncl,nch);
    //free_bmatrix(I2,nrl,nrh,ncl,nch);

    for(i = 0 ; i < nrh ; i++){
        for(j = 0 ; j < nch ; j++){
            if(regions[i][j] != etiquette){
                I3[i][j] = 0 ;
            }
        }
    }

    SavePGM_bmatrix(I3,nrl,nrh,ncl,nch,"rice_etiq_extraction.pgm");

}

void rechercher_et_caracteriser_region(int etiquette){

}


int main (){

    //binarisation(180);
    etiquetage(127, 200);

    return 0 ;
}
