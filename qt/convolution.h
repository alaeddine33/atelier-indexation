#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Lib/def.h"
#include "../Lib/nrio.h"
#include "../Lib/nrarith.h"
#include "../Lib/nralloc.h"

byte** gradientNorm(byte** gradX, byte**gradY, int nrl, int nrh, int ncl, int nch);
float meanGradientNorm(byte** gradient, int nrl, int nrh, int ncl, int nch);
float MGN_from_image(byte** img, int nrl, int nrh, int ncl, int nch);
float MGN_from_file(char* name);
int edgePixels(byte** gradient, int nrl, int nrh, int ncl, int nch);
int EP_from_image(byte** img, int nrl, int nrh, int ncl, int nch);
int EP_from_file(char* name);
byte** convolution(byte** orig, int mask[3][3], int nrl, int nrh, int ncl, int nch, int fraction);
byte** binarization(byte** orig, byte threshold, int nrl, int nrh, int ncl, int nch);

#endif
