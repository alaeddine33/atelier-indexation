#ifndef VECTEUR_H
#define VECTEUR_H

#include "../Lib/def.h"
#include "../Lib/nrio.h"
#include "../Lib/nrarith.h"
#include "../Lib/nralloc.h"

/**
 *  Fonction de traitement d'images
 */
byte** gradientNorm(byte** gradX, byte**gradY, int nrl, int nrh, int ncl, int nch);
float meanGradientNorm(byte** gradient, int nrl, int nrh, int ncl, int nch);
float MGN_from_image(byte** img, int nrl, int nrh, int ncl, int nch);
float MGN_from_file(char* name);
int edgePixels(byte** gradient, int nrl, int nrh, int ncl, int nch);
int EP_from_image(byte** img, int nrl, int nrh, int ncl, int nch);
int EP_from_file(char* name);
byte** convolution(byte** orig, int mask[3][3], int nrl, int nrh, int ncl, int nch, int fraction);
byte** binarization(byte** orig, byte threshold, int nrl, int nrh, int ncl, int nch);
long* histogram(byte** image, long nrl, long nrh, long ncl, long nch);
long* histogram_cumule(byte** image, long nrl, long nrh, long ncl, long nch);
long** histogram_rgb(rgb8** image, long nrl, long nrh, long ncl, long nch);
long* histogram_file(char* file_image);
byte** rgb_to_greyscale(rgb8** image, long nrl, long nrh, long ncl, long nch);
float* rgb_rate(rgb8** image, long nrl, long nrh, long ncl, long nch);
float* rgb_rate_file(char* file_image);

/**
 *  Fonction de génération de script pl/sql
 */
void generate_plsql_script(char* file, char* image_name);

#endif
