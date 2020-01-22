#include "distance.h"

long* difference_histogram(long* histogram1, long* histogram2){

  int i;
  long* diff = (long*)malloc(256*sizeof(long));

  for(i=0; i<256; i++){
    diff[i] = histogram1[i] - histogram2[i];
  }

  return diff;
}

/**
 *
 *
 */
double euclidian_norm(double* histogram1, double* histogram2){
  int i;
  float diff = 0.0;
  double hists1=0.0;
  double hists2=0.0;

  for(i=0; i<256; i++){
    hists1 =pow(histogram1[i],2);
    hists2 =pow(histogram2[i],2);

    diff = diff + sqrt(fabs(hists2 - hists1));

  }

  return diff;
}

double bhattacharyya(double* histogram1, double* histogram2){
  int i;
  float diff = 0.0;
  double hists1=0.0;
  double hists2=0.0;

  for(i=0; i<256; i++){
    hists1 =pow(histogram1[i],2);
    hists2 =pow(histogram2[i],2);

    diff = diff + sqrt(hists2 * hists1);

  }

  return sqrt(1-diff);
}

void display_histogram(int* histogram){

}

double* reduce_histogram(long* histogram,long nrl, long nrh, long ncl, long nch){

  double* hist = (double*)malloc(256*sizeof(double));
  int i=0;
  double total=0;
  for(i=0;i<256;i++){
    hist[i] = ((double)histogram[i]/((nrh+1)*(nch+1)));
    total = total + hist[i];
  }

  return hist;
}

float compare(rgb8** image1_in, rgb8** image2_in,long nrl, long nrh, long ncl, long nch,long nrl1, long nrh1, long ncl1, long nch1){

  byte** image1 = rgb_to_greyscale(image1_in, nrl, nrh,ncl,nch);
  byte** image2 = rgb_to_greyscale(image2_in, nrl1, nrh1,ncl1,nch1);

  double* histogram1;
  double* histogram2;

  double score_histogram = 0.0;
  /*float score_MGN = 0.0;
  float score_nbrPixel_Contour = 0.0;
  float score_rgb[3];

  float* rate_rgb = (float*)malloc(3*sizeof(float));
  float* rate_rgb1 = (float*)malloc(3*sizeof(float));
  */
  //get and compare histogram
  histogram1 = reduce_histogram(histogram(image1,nrl,nrh,ncl,nch),nrl,nrh,ncl,nch);
  histogram2 = reduce_histogram(histogram(image2,nrl1,nrh1,ncl1,nch1),nrl1,nrh1,ncl1,nch1);
  score_histogram = euclidian_norm(histogram1, histogram2);
  //score_histogram = bhattacharyya(histogram1, histogram2);

  //get and compare norm score mean
  //score_MGN = abs(MGN_from_image(image1,nrl,nrh,ncl,nch)-MGN_from_image(image2,nrl1,nrh1,ncl1,nch1));

  //get and compare nbrPixel edges
  //score_nbrPixel_Contour = abs(EP_from_image(image1,nrl,nrh,ncl,nch)-EP_from_image(image2,nrl1,nrh1,ncl1,nch1));

  //rate of red, green and blue
  //rate_rgb = rgb_rate(image1_in,nrl,nrh,ncl, nch);
  //rate_rgb1 = rgb_rate(image2_in,nrl1,nrh1,ncl1, nch1);

  //score_rgb[0] = abs(rate_rgb[0] - rate_rgb1[0]);
  //score_rgb[1] = abs(rate_rgb[1] - rate_rgb1[1]);
  //score_rgb[2] = abs(rate_rgb[2] - rate_rgb1[2]);


  //printf("total = %f\n",score_histogram);

  return score_histogram;

  }

  //compare_all(image, nrl, nrh,ncl, nch, "./archive500/ppm/",times,score);


  char* compare_all(rgb8** image1_in, long nrl, long nrh, long ncl, long nch, char* directory, double* timese, double* scoreMin){

    //declare
    int i;
    DIR* dir = NULL ;
    struct dirent* actualFile = NULL ;
    char* actualFilePath = (char*)malloc(100*sizeof(char)) ;
    long nrl_cp, nrh_cp, ncl_cp, nch_cp;
    rgb8** image;
    byte** image_gs;
    double score=20;
    double scorem=20;
    double timem=20;

    *scoreMin=20;
    *timese=0;

    char* finalMin = (char*)malloc(100*sizeof(char));
    clock_t start, stop;
    double elapsed;

    start = clock();
    //open directory
    dir = opendir(directory);

    if(dir == NULL){
        printf("Erreur ouverture du répertoire.") ;
        exit(1);
    }

    //browse all images
    while((actualFile = readdir(dir)) != NULL){
      if(strcmp(actualFile->d_name, "..") && strcmp(actualFile->d_name, ".")){

        sprintf(actualFilePath, "./archive500/ppm/%s", actualFile->d_name);
        image = LoadPPM_rgb8matrix(actualFilePath, &nrl_cp, &nrh_cp, &ncl_cp, &nch_cp);

        //compare
        if((score = compare(image,image1_in,nrl_cp,nrh_cp,ncl_cp,nch_cp,nrl,nrh,ncl,nch)) < *scoreMin ){
          *scoreMin = score;
          strcpy (finalMin, actualFile->d_name);
        }

      }
    }

    stop = clock();
    elapsed = ((double)stop - start) / CLOCKS_PER_SEC;
    
    *timese = elapsed;

    return finalMin;

  }
