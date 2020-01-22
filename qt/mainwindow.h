#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "feature_extractor.h"
#include "convolution.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DIR* dir = NULL ;
    FILE* script = NULL ;
    long* histogramme ;
    float mgn = 0.0 ;
    float* rate ;
    int nb_pixel_contour = 0 ;

    std::string images_dir = "../src/archive500/ppm/";
    std::string image_name;
    std::string image_path;
    rgb8** image ;
    byte** image_nb ;
    long nrh, nrl, nch, ncl;

private slots:
    void on_fileButton_clicked();

    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
