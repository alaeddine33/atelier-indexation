#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <string.h>


using namespace std;

std::vector<string> split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

string exec();
string GetStdoutFromCommand(string cmd);
double euclidian_norm(double* histogram1, double* histogram2);
void display_histogram(int* histogram);
double bhattacharyya(double* histogram1, double* histogram2);

float compare(rgb8** image1_in, rgb8** image2_in,long nrl, long nrh, long ncl, long nch,long nrl1, long nrh1, long ncl1, long nch1);
char* compare_all(rgb8** image1_in, long nrl, long nrh, long ncl, long nch, char* directory, double* time, double* scoreMin, char* remove);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Ouvrir image"), "./", "Images (*.jpg *.ppm *.pgm)");
    image_path = filename.toStdString();
    int index = image_path.find_last_of("/\\");
    image_name = image_path.substr(index+1);
    ui->fileLabel->setText(QString::fromStdString(image_name));

    QPixmap inputPixmap;
    inputPixmap.load(filename);
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->addPixmap(inputPixmap);
    ui->inputImg->setScene(scene);
    ui->inputImg->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_startButton_clicked()
{
    image = LoadPPM_rgb8matrix(const_cast<char*> (image_path.c_str()), &nrl, &nrh, &ncl, &nch);

    double time = 0, score = 0;
    char* name = compare_all(image, nrl, nrh, ncl, nch, const_cast<char*> (images_dir.c_str()), &time, &score, const_cast<char*> (image_name.c_str()));

    cout << name << endl << score << endl << time << endl;

    QPixmap resultPixmap;
    resultPixmap.load(QString::fromStdString("../src/archive500/ppm/"+string(name)));
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->addPixmap(resultPixmap);
    ui->ourImg->setScene(scene);
    ui->ourImg->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    ostringstream timeStream;
    timeStream << "Temps de calcul : " << time << " s";
    ostringstream resStream;
    resStream << "Ressemblance : " << (1-score)*100 << " %";
    ui->calcTimeUs->setText(QString::fromStdString(timeStream.str()));
    ui->simUs->setText(QString::fromStdString(resStream.str()));

    FILE *in;
    char buff[512];

    char temp[10];
    strcpy(temp, image_name.c_str());
    string tempS = string(temp);

    ostringstream cmd;
    cmd << "echo \"/\\n'" << tempS.replace(tempS.end()-3, tempS.end(), "jpg") << "'\\n\" | sqlplus64 VINCMONO/VINCMONO17@\"(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.40.128.30)(PORT=1521))(CONNECT_DATA=(SID=emrepus)))\" @/home/agurato/dev/m2/indexation/src/comparison_2.sql";

    cout << cmd.str() << endl;
    ostringstream output;
    /*
    if(!(in = popen("echo \"/\\n\" | sqlplus64 VINCMONO/VINCMONO17@\"(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.40.128.30)(PORT=1521))(CONNECT_DATA=(SID=emrepus)))\" @/home/agurato/dev/m2/indexation/src/comparison_2.sql", "r"))){
    }
    */
    if(!(in = popen(cmd.str().c_str(), "r"))){
    }
    while(fgets(buff, sizeof(buff), in)!=NULL){
        output << buff;
    }
    pclose(in);

    string outputS = output.str();

    ostringstream timeOStream;
    ostringstream resOStream;

    std::vector<string> arr;
    arr=split(outputS, "\n");
    for(size_t i=0;i<arr.size();i++) {
        if(arr[i].find("img=") != string::npos) {
            cout << "../src/archive500/"+arr[i].substr(4) << endl;
            QPixmap resultOPixmap;
            resultOPixmap.load(QString::fromStdString("../src/archive500/"+arr[i].substr(4)));
            QGraphicsScene* scene = new QGraphicsScene(this);
            scene->addPixmap(resultOPixmap);
            ui->oracleImg->setScene(scene);
            ui->oracleImg->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        }
        else if(arr[i].find("diff=") != string::npos) {
            timeOStream << "Temps de calcul : " << arr[i].substr(5) << " s";
            ui->calcTimeO->setText(QString::fromStdString(timeOStream.str()));
        }
        else if(arr[i].find("time=") != string::npos) {
            resOStream << "Ressemblance : " << (1-atof(arr[i].substr(5).c_str()))*100 << " %";
            ui->simO->setText(QString::fromStdString(resOStream.str()));
        }
    }
}

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


  char* compare_all(rgb8** image1_in, long nrl, long nrh, long ncl, long nch, char* directory, double* timese, double* scoreMin, char* remove){

    //declare
    int i;
    DIR* dir = NULL ;
    struct dirent* actualFile = NULL ;
    char* actualFilePath = (char*)malloc(100*sizeof(char)) ;
    long nrl_cp, nrh_cp, ncl_cp, nch_cp;
    rgb8** image;
    byte** image_gs;
    double score = 0;

    *scoreMin=20;

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
      if(strcmp(actualFile->d_name, "..") && strcmp(actualFile->d_name, ".") && strcmp(actualFile->d_name, remove)){

        sprintf(actualFilePath, "%s%s", directory, actualFile->d_name);
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
