#include "convolution.h"

int main() {
    /*
    // nrh: number of rows high
    long nrl = 0, nrh = 0, ncl = 0, nch = 0;

    int mask_lowpass[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    int mask_gradX[3][3] = {{-1, 0, +1}, {-2, 0, +2}, {-1, 0, +1}};
    int mask_gradY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {+1, +2, +1}};

    byte** origImage = LoadPGM_bmatrix("ImagesTests/rice.pgm", &nrl, &nrh, &ncl, &nch);

    // Filtre pass-bas (flou)
    byte** lowpass = convolution(origImage, mask_lowpass, nrl, nrh, ncl, nch, 9);
    SavePGM_bmatrix(lowpass, nrl, nrh, ncl, nch, "output_img/rice_lowpass.pgm");

    // Gradient X et Y
    byte** gradX = convolution(origImage, mask_gradX, nrl, nrh, ncl, nch, 2);
    byte** gradY = convolution(origImage, mask_gradY, nrl, nrh, ncl, nch, 2);
    byte** gradient = gradientNorm(gradX, gradY, nrl, nrh, ncl, nch);
    byte** gradient_bin = binarization(gradient, 70, nrl, nrh, ncl, nch);

    int pixel_nb = ((nrh-nrl-1)*(nch-ncl-1));

    printf("moyenne norme gradient = %f\n", meanGradientNorm(gradient, nrl, nrh, ncl, nch));

    int edgePixelNb = edgePixels(gradient_bin, nrl, nrh, ncl, nch);
    printf("nombre de pixels de contour = %d/%d : %f %%\n", edgePixelNb, pixel_nb, 100*( (float) edgePixelNb/pixel_nb));

    SavePGM_bmatrix(gradX, nrl, nrh, ncl, nch, "output_img/rice_edge_x.pgm");
    SavePGM_bmatrix(gradY, nrl, nrh, ncl, nch, "output_img/rice_edge_y.pgm");
    SavePGM_bmatrix(gradient, nrl, nrh, ncl, nch, "output_img/rice_edge.pgm");
    SavePGM_bmatrix(gradient_bin, nrl, nrh, ncl, nch, "output_img/rice_edge_bin.pgm");

    free_bmatrix(origImage, nrl, nrh, ncl, nch);
    free_bmatrix(gradX, nrl, nrh, ncl, nch);
    free_bmatrix(gradY, nrl, nrh, ncl, nch);
    free_bmatrix(gradient, nrl, nrh, ncl, nch);
    free_bmatrix(gradient_bin, nrl, nrh, ncl, nch);
    */

    printf("%f\n", MGN_from_file("ImagesTests/rice.pgm"));
    printf("%d\n", EP_from_file("ImagesTests/rice.pgm"));

    return 0;
}
