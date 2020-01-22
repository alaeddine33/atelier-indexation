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
void prise_en_main(){
  long nrh,nrl,nch,ncl;
  byte **R;

  R = LoadPGM_bmatrix("cubesx3.pgm",&nrl,&nrh,&ncl,&nch);

  //R = bmatrix(nrl,nrh,ncl,nch);


  SavePGM_bmatrix(R,nrl,nrh,ncl,nch,"imageTest.pgm");

  free_bmatrix(R,nrl,nrh,ncl,nch);
}

/**

  TP1
  convolution filtre passe bas

**/
void convolution_passe_bas(){
  int i, j;
  long nrh,nrl,nch,ncl;
	byte **R, **R2;
  int masque_moyen[3][3] = {{1,1,1},{1,1,1},{1,1,1}};

  R = LoadPGM_bmatrix("cubesx3.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl,nrh,ncl,nch);

  printf("nch = %ld\n", nch);
  printf("nch = %ld\n", nrh);


  // Filtrage

  int un_un = masque_moyen[0][0];
  int un_deux = masque_moyen[0][1];
  int un_trois = masque_moyen[0][2];
  int deux_un = masque_moyen[1][0];
  int deux_deux = masque_moyen[1][1];
  int deux_trois = masque_moyen[1][2];
  int trois_un = masque_moyen[2][0];
  int trois_deux = masque_moyen[2][1];
  int trois_trois = masque_moyen[2][2];

  for (i = 1 ; i < nrh ; i++){
    for(j = 1 ; j < nch ; j++){
      int int_pixel_value = ((un_un*R[i-1][j-1])+(un_deux*R[i-1][j])+(un_trois*R[i-1][j+1])+(deux_un*R[i][j-1])+(deux_deux*R[i][j])+(deux_trois*R[i][j+1])+(trois_un*R[i+1][j-1])+(trois_deux*R[i+1][j])+(trois_trois*R[i+1][j+1]))/9;
      byte byte_pixel_value = (byte)int_pixel_value;
      R2[i][j] = byte_pixel_value;
    }
  }


  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"cubesmoyenne.pgm");

  free_bmatrix(R,nrl,nrh,ncl,nch);
  free_bmatrix(R2,nrl,nrh,ncl,nch);

}

void gradient_horizontal(){
  int i, j;
  long nrh,nrl,nch,ncl;
	byte **R, **R2;
  int masque_moyen[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};

  R = LoadPGM_bmatrix("cubesmoyenne.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl,nrh,ncl,nch);

  printf("nch = %ld\n", nch);
  printf("nch = %ld\n", nrh);


  // Filtrage

  int un_un = masque_moyen[0][0];
  int un_deux = masque_moyen[0][1];
  int un_trois = masque_moyen[0][2];
  int deux_un = masque_moyen[1][0];
  int deux_deux = masque_moyen[1][1];
  int deux_trois = masque_moyen[1][2];
  int trois_un = masque_moyen[2][0];
  int trois_deux = masque_moyen[2][1];
  int trois_trois = masque_moyen[2][2];

  for (i = 1 ; i < nrh ; i++){
    for(j = 1 ; j < nch ; j++){

      int int_pixel_value = ((un_un*R[i-1][j-1])+(un_deux*R[i-1][j])+(un_trois*R[i-1][j+1])+(deux_un*R[i][j-1])+(deux_deux*R[i][j])+(deux_trois*R[i][j+1])+(trois_un*R[i+1][j-1])+(trois_deux*R[i+1][j])+(trois_trois*R[i+1][j+1]))/4;
      if(int_pixel_value < 0) {
        int_pixel_value = int_pixel_value*-1;
      }
      byte byte_pixel_value = (byte)int_pixel_value;
      R2[i][j] = byte_pixel_value;
    }
  }


  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"cubesgradienthorizontal.pgm");

  free_bmatrix(R,nrl,nrh,ncl,nch);
  free_bmatrix(R2,nrl,nrh,ncl,nch);

}

void gradient_vertical(){
  int i, j;
  long nrh,nrl,nch,ncl;
	byte **R, **R2;
  int masque_moyen[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};

  R = LoadPGM_bmatrix("cubesmoyenne.pgm",&nrl,&nrh,&ncl,&nch);
  R2 = bmatrix(nrl,nrh,ncl,nch);

  // Filtrage

  int un_un = masque_moyen[0][0];
  int un_deux = masque_moyen[0][1];
  int un_trois = masque_moyen[0][2];
  int deux_un = masque_moyen[1][0];
  int deux_deux = masque_moyen[1][1];
  int deux_trois = masque_moyen[1][2];
  int trois_un = masque_moyen[2][0];
  int trois_deux = masque_moyen[2][1];
  int trois_trois = masque_moyen[2][2];

  for (i = 1 ; i < nrh ; i++){
    for(j = 1 ; j < nch ; j++){

      int int_pixel_value = ((un_un*R[i-1][j-1])+(un_deux*R[i-1][j])+(un_trois*R[i-1][j+1])+(deux_un*R[i][j-1])+(deux_deux*R[i][j])+(deux_trois*R[i][j+1])+(trois_un*R[i+1][j-1])+(trois_deux*R[i+1][j])+(trois_trois*R[i+1][j+1]))/4;
      if(int_pixel_value < 0) {
        int_pixel_value = int_pixel_value*-1;
      }
      byte byte_pixel_value = (byte)int_pixel_value;
      R2[i][j] = byte_pixel_value;
    }
  }


  SavePGM_bmatrix(R2,nrl,nrh,ncl,nch,"cubesgradientvertical.pgm");

  free_bmatrix(R,nrl,nrh,ncl,nch);
  free_bmatrix(R2,nrl,nrh,ncl,nch);

}

void norme_gradient(){
  int i, j;
  long nrh,nrl,nch,ncl;
	byte **R, **R_vertical, **R_horizontal;

  R_vertical = LoadPGM_bmatrix("cubesgradientvertical.pgm",&nrl,&nrh,&ncl,&nch);
  R_horizontal = LoadPGM_bmatrix("cubesgradienthorizontal.pgm",&nrl,&nrh,&ncl,&nch);
  R = bmatrix(nrl,nrh,ncl,nch);
  int pixel = 0 ;

  for (i = 1 ; i < nrh ; i++){
    for(j = 1 ; j < nch ; j++){
       pixel = sqrt(((int)R_vertical[i][j]*(int)R_vertical[i][j])+((int)R_horizontal[i][j]*(int)R_horizontal[i][j]));
       if(pixel < 255 && pixel > 25){
          R[i][j] = pixel ;
       }
    }
  }

  SavePGM_bmatrix(R,nrl,nrh,ncl,nch,"cubesgradient.pgm");

  free_bmatrix(R,nrl,nrh,ncl,nch);
  free_bmatrix(R_vertical,nrl,nrh,ncl,nch);
  free_bmatrix(R_horizontal,nrl,nrh,ncl,nch);

}

int main(void){
  // 2
  //prise_en_main();

  // 3
  //convolution_passe_bas();

  // 4
  // 4.1
  //gradient_horizontal();

  // 4.2
  //gradient_vertical();

  // 4.3
  norme_gradient();

  return 0 ;
}
