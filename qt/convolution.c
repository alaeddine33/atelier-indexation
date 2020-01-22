#include "convolution.h"

byte** gradientNorm(byte** gradX, byte**gradY, int nrl, int nrh, int ncl, int nch) {
    int i = 0, j = 0;
    byte** gradient = bmatrix(nrl, nrh, ncl, nch);
    for(i=nrl+1 ; i<nrh ; i++) {
        for(j=ncl+1 ; j<nch ; j++) {
            double squaredX = pow((double) gradX[i][j], 2);
            double squaredY = pow((double) gradY[i][j], 2);
            int pixel = sqrt(squaredX + squaredY);
            gradient[i][j] = (byte) pixel;
        }
    }
    return gradient;
}

float meanGradientNorm(byte** gradient, int nrl, int nrh, int ncl, int nch) {
    int pixel_nb = ((nrh-nrl-1)*(nch-ncl-1));
    int sum = 0, i = 0, j = 0;
    for(i=nrl+1 ; i<nrh ; i++) {
        for(j=ncl+1 ; j<nch ; j++) {
            sum += gradient[i][j];
        }
    }
    return (float) sum/pixel_nb;
}

float MGN_from_image(byte** img, int nrl, int nrh, int ncl, int nch) {
    int mask_gradX[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int mask_gradY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}};

    byte** gradX = convolution(img, mask_gradX, nrl, nrh, ncl, nch, 2);
    byte** gradY = convolution(img, mask_gradY, nrl, nrh, ncl, nch, 2);
    byte** gradient = gradientNorm(gradX, gradY, nrl, nrh, ncl, nch);

    float mean =  meanGradientNorm(gradient, nrl, nrh, ncl, nch);

    free_bmatrix(gradX, nrl, nrh, ncl, nch);
    free_bmatrix(gradY, nrl, nrh, ncl, nch);
    free_bmatrix(gradient, nrl, nrh, ncl, nch);

    return mean;
}

float MGN_from_file(char* name) {
    long nrl = 0, nrh = 0, ncl = 0, nch = 0;
    int mask_gradX[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int mask_gradY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}};

    byte** origImage = LoadPGM_bmatrix(name, &nrl, &nrh, &ncl, &nch);

    byte** gradX = convolution(origImage, mask_gradX, nrl, nrh, ncl, nch, 2);
    byte** gradY = convolution(origImage, mask_gradY, nrl, nrh, ncl, nch, 2);
    byte** gradient = gradientNorm(gradX, gradY, nrl, nrh, ncl, nch);

    float mean =  meanGradientNorm(gradient, nrl, nrh, ncl, nch);

    free_bmatrix(origImage, nrl, nrh, ncl, nch);
    free_bmatrix(gradX, nrl, nrh, ncl, nch);
    free_bmatrix(gradY, nrl, nrh, ncl, nch);
    free_bmatrix(gradient, nrl, nrh, ncl, nch);

    return mean;
}

int edgePixels(byte** gradient, int nrl, int nrh, int ncl, int nch) {
    int sum = 0, i = 0, j = 0;
    for(i=nrl+1 ; i<nrh ; i++) {
        for(j=ncl+1 ; j<nch ; j++) {
            sum += gradient[i][j];
        }
    }
    return sum / 255;
}

int EP_from_image(byte** img, int nrl, int nrh, int ncl, int nch) {
    int mask_gradX[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int mask_gradY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}};

    byte** gradX = convolution(img, mask_gradX, nrl, nrh, ncl, nch, 2);
    byte** gradY = convolution(img, mask_gradY, nrl, nrh, ncl, nch, 2);
    byte** gradient = gradientNorm(gradX, gradY, nrl, nrh, ncl, nch);
    byte** gradient_bin = binarization(gradient, 70, nrl, nrh, ncl, nch);

    int nb = edgePixels(gradient_bin, nrl, nrh, ncl, nch);

    free_bmatrix(gradX, nrl, nrh, ncl, nch);
    free_bmatrix(gradY, nrl, nrh, ncl, nch);
    free_bmatrix(gradient, nrl, nrh, ncl, nch);
    free_bmatrix(gradient_bin, nrl, nrh, ncl, nch);

    return nb;
}

int EP_from_file(char* name) {
    long nrl = 0, nrh = 0, ncl = 0, nch = 0;
    int mask_gradX[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int mask_gradY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}};

    byte** origImage = LoadPGM_bmatrix(name, &nrl, &nrh, &ncl, &nch);

    byte** gradX = convolution(origImage, mask_gradX, nrl, nrh, ncl, nch, 2);
    byte** gradY = convolution(origImage, mask_gradY, nrl, nrh, ncl, nch, 2);
    byte** gradient = gradientNorm(gradX, gradY, nrl, nrh, ncl, nch);
    byte** gradient_bin = binarization(gradient, 70, nrl, nrh, ncl, nch);

    int nb = edgePixels(gradient_bin, nrl, nrh, ncl, nch);

    free_bmatrix(origImage, nrl, nrh, ncl, nch);
    free_bmatrix(gradX, nrl, nrh, ncl, nch);
    free_bmatrix(gradY, nrl, nrh, ncl, nch);
    free_bmatrix(gradient, nrl, nrh, ncl, nch);
    free_bmatrix(gradient_bin, nrl, nrh, ncl, nch);

    return nb;
}

byte** convolution(byte** orig, int mask[3][3], int nrl, int nrh, int ncl, int nch, int fraction) {
    int i = 0, j = 0;
    byte** transformed = bmatrix(nrl, nrh, ncl, nch);

    for(i=nrl+1 ; i<nrh ; i++) {
        for(j=ncl+1 ; j<nch ; j++) {
            int pixel =
                (orig[i-1][j-1]*mask[0][0] + orig[i-1][j]*mask[0][1] + orig[i-1][j+1]*mask[0][2]
                + orig[i][j-1]*mask[1][0] + orig[i][j]*mask[1][1] + orig[i][j+1]*mask[1][2]
                + orig[i+1][j-1]*mask[2][0] + orig[i+1][j]*mask[2][1] + orig[i+1][j+1]*mask[2][2])/fraction;
            transformed[i][j] = (byte) abs(pixel);
        }
    }

    return transformed;
}

byte** binarization(byte** orig, byte threshold, int nrl, int nrh, int ncl, int nch) {
    int i = 0, j = 0;
    byte **transformed = bmatrix(nrl, nrh, ncl, nch);

    for(i = 0 ; i < nrh ; i++) {
        for (j = 0 ; j < nch ; j++) {
            if(orig[i][j] < threshold) {
                transformed[i][j] = (byte) 0;
            }
            else {
                transformed[i][j] = (byte) 255;
            }
        }
    }

    return transformed;
}
