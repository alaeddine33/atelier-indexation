#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "convolution.h"
#include "feature_extractor.h"
#include <dirent.h>
#include <time.h>

long* difference_histogram(long* histogram1, long* histogram2);
double euclidian_norm(double* histogram1, double* histogram2);
void display_histogram(int* histogram);
double bhattacharyya(double* histogram1, double* histogram2);

float compare(rgb8** image1_in, rgb8** image2_in,long nrl, long nrh, long ncl, long nch,long nrl1, long nrh1, long ncl1, long nch1);
char* compare_all(rgb8** image1_in, long nrl, long nrh, long ncl, long nch, char* directory, double* time, double* scoreMin);
#endif
