#include "feature_extractor.h"

#include <math.h>

/**
 *
 *
 */
long* histogram(byte** image, long nrl, long nrh, long ncl, long nch){
    int i, j ;
    long* histogramme = (long*)calloc(256, sizeof(long)) ;

    for(i = nrl ; i < nrh+1 ; i++){
        for(j = ncl ; j < nch+1 ; j++){
            histogramme[image[i][j]]++ ;

        }
    }

    /*for(i = 0 ; i < 256 ; i++){
        printf("%d\n", histogramme[i]);
    }*/
    return histogramme ;
}

long* histogram_file(char* file_image){
    long nrh,nrl,nch,ncl ;
    rgb8** image ;
    byte** image_nb ;
    long* histogramme ;

    image = LoadPPM_rgb8matrix(file_image, &nrl, &nrh, &ncl, &nch);
    image_nb = rgb_to_greyscale(image, nrl, nrh, ncl, nch);

    histogramme = histogram(image_nb, nrl, nrh, ncl, nch);

    // free image à faire
    return histogramme ;
}

/**
 *
 *
 */
long* histogram_cumule(byte** image, long nrl, long nrh, long ncl, long nch){
    int i, j ;
    long* histogramme_cumule = (long*)malloc(256*sizeof(long)) ;

    // Histogramme classique
    for(i = nrl ; i < nrh+1 ; i++){
        for(j = ncl ; j < nch+1 ; j++){
            histogramme_cumule[image[i][j]]++ ;
        }
    }

    // Cumulation
    for(i = 0 ; i < 256 ; i++){
        for(j = 0 ; j < i ; j++){
            histogramme_cumule[i] += histogramme_cumule[j] ;
        }
    }

    return histogramme_cumule ;
}

long** histogram_rgb(rgb8** image, long nrl, long nrh, long ncl, long nch){
    int i, j ;

    long** histogram = (long**)malloc(3*sizeof(long*));

    for(i = 0 ; i < 3 ; i++){
        histogram[i] = (long*)malloc(256*sizeof(long));
    }

    for(i = nrl ; i < nrh+1 ; i++){
        for(j = ncl ; j < nch+1 ; j++){
            histogram[0][image[i][j].r]++ ;
            histogram[1][image[i][j].g]++ ;
            histogram[2][image[i][j].b]++ ;
        }
    }

    return histogram ;
}

/**
 *
 *
 */
byte** rgb_to_greyscale(rgb8** image, long nrl, long nrh, long ncl, long nch){
    byte** I = bmatrix(nrl,nrh,ncl,nch);
    int i, j ;
    byte r, g, b ;

    for(i = nrl ; i < nrh+1 ; i++){
        for(j = ncl ; j < nch+1 ; j++){
            r = image[i][j].r ;
            g = image[i][j].g ;
            b = image[i][j].b ;
            I[i][j] = (r + g + b)/3 ;
        }
    }

    return I ;
}

/**
 *
 *
 */
float* rgb_rate(rgb8** image, long nrl, long nrh, long ncl, long nch){
    int i, j ;
    long r = 0, g = 0, b = 0, total = 0 ;

    float* rate = (float*)malloc(3*sizeof(float));

    for(i = nrl ; i < nrh+1 ; i++){
        for(j = ncl ; j < nch+1 ; j++){
            r += image[i][j].r ;
            g += image[i][j].g ;
            b += image[i][j].b ;
        }
    }

    total = r+g+b ;

    rate[0] = (float)r/(float)total ; // red rate
    rate[1] = (float)g/(float)total ; // green rate
    rate[2] = (float)b/(float)total ; // blue rate

    return rate ;
}

float* rgb_rate_file(char* file_image){
    long nrh,nrl,nch,ncl ;
    rgb8** image ;
    float* rate ;

    image = LoadPPM_rgb8matrix(file_image, &nrl, &nrh, &ncl, &nch);

    rate = rgb_rate(image, nrl, nrh, ncl, nch);

    // free image à faire
    return rate ;

}
