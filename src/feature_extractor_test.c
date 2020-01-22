#include "feature_extractor.h"
#include "distance.h"
// gcc -o feature_extractor_test ../Lib/nralloc.c ../Lib/nrio.c ../Lib/nrarith.c feature_extractor.c feature_extractor_test.c -w -lm

int main(){
    long nrh,nrl,nch,ncl ;
    long nrh1,nrl1,nch1,ncl1 ;

    rgb8** image ;
    rgb8** image1 ;

    byte** image_nb ;
    byte** image_nb_2 ;
    long* histo ;
    int i = 0 ;
    /*
    image = LoadPPM_rgb8matrix("./archive10/ppm/image0.ppm",&nrl,&nrh,&ncl,&nch);
    image_nb = rgb_to_greyscale(image, nrl, nrh, ncl, nch);

    SavePGM_bmatrix(image_nb,nrl,nrh,ncl,nch,"./archive10/ppm/image1_gsa.pgm");
    free_bmatrix(image_nb, nrl, nrh, ncl, nch);
    */

    /*image_nb_2 = LoadPGM_bmatrix("./archive500/pgm/0.pgm",&nrl,&nrh,&ncl,&nch);
    histo = histogram(image_nb_2, nrl, nrh, ncl, nch);

    for(i = 0 ; i < 256 ; i++){
        printf("histo[%d] = %ld\n", i, histo[i]);
    }*/

    image = LoadPPM_rgb8matrix("./archive10/ppm/image0.ppm",&nrl,&nrh,&ncl,&nch);
    image1 = LoadPPM_rgb8matrix("./archive10/ppm/image1.ppm",&nrl1,&nrh1,&ncl1,&nch1);

    compare(image, image1, nrl,  nrh,  ncl,  nch, nrl1,  nrh1,  ncl1,  nch1);


    return 0 ;

}
