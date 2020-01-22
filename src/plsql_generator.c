#include "plsql_generator.h"
#include "feature_extractor.h"
#include "convolution.h"

#include <dirent.h>
#include <string.h>

// gcc -o plsql_generator plsql_generator.c convolution.c feature_extractor.c ../Lib/nralloc.c ../Lib/nrio.c ../Lib/nrarith.c -w -lm

void generate_plsql_script(char* directory, char* script_file){
    // variable de fichiers
    DIR* dir = NULL ;
    struct dirent* actualFile = NULL ;
    char* actualFilePath = (char*)malloc(100*sizeof(char)) ;
    FILE* script = NULL ;
    char* fileNameJPG = (char*)malloc(100*sizeof(char));
    long nrh,nrl,nch,ncl ;
    rgb8** image ;
    byte** image_nb ;

    // variables de char* pour les requêtes
    char* actualFileName = (char*)malloc(100*sizeof(char));
    char* requestRGBRate = (char*)malloc(100*sizeof(char));
    char* requestMNG = (char*)malloc(100*sizeof(char));
    char* requestHistogram = (char*)malloc(15000*sizeof(char));
    char* requestNPC = (char*)malloc(100*sizeof(char));

    // autres variables
    int i_histo = 0 ;
    char* actualHisto = (char*)malloc(10*sizeof(char));
    long* histogramme ;
    float mgn = 0.0 ;
    float* rate ;
    int nb_pixel_contour = 0 ;


    // Ouverture du fichier de script
    script = fopen(script_file, "w+");

    if(script != NULL){
        // Ouverture du répertoire
        dir = opendir(directory);

        // Test de l'ouverture du répertoire
        if(dir == NULL){
            printf("Erreur ouverture du répertoire.") ;
            exit(1);
        }

        // Sur plusieurs ligne pour la lisibilité mais à mettre dans un unique
        // fputs().
        fputs("declare\n", script);
        fputs("\th HISTO_TYPE;\n", script);
        fputs("\tm FLOAT;\n", script);
        fputs("\tn NUMERIC;\n", script);
        fputs("\tt TAUX_TYPE;\n", script);
        fputs("begin\n\n", script);
        // à modifier à partir d'ici


        /**
        *   select column1, column2 into variable1, variable2
        *
        *
        **/
        while((actualFile = readdir(dir)) != NULL){
            if(strcmp(actualFile->d_name, "..") && strcmp(actualFile->d_name, ".")){
                // Chargement image ppm
                sprintf(actualFilePath, "./archive500/ppm/%s", actualFile->d_name);
                image = LoadPPM_rgb8matrix(actualFilePath, &nrl, &nrh, &ncl, &nch);
                image_nb = rgb_to_greyscale(image, nrl, nrh, ncl, nch);

                // Remplacement nom pour BDD avec l'extension jpg
                strcpy(fileNameJPG, actualFile->d_name);
                fileNameJPG[strlen(fileNameJPG)-3] = 'j' ;
                fileNameJPG[strlen(fileNameJPG)-2] = 'p' ;
                fileNameJPG[strlen(fileNameJPG)-1] = 'g' ;

                //printf("Le fichier lu s'appelle %s.\n", actualFile->d_name);

                // Début de la requête
                fputs("\tselect HISTOGRAMME, MOYENNE_NORME_GRADIENT, NB_PIXEL_COUNTOUR, TAUX_COULEURS into h, m, n, t\n", script);
                fputs("\tfrom multimedia\n", script);

                sprintf(actualFileName, "\twhere nom = '%s'\n", fileNameJPG);
                fputs(actualFileName, script);

                fputs("\tfor update;\n", script);

                // Calcul de l'histogramme et génération requête
                histogramme = histogram(image_nb, nrl, nrh, ncl, nch);
                strcpy(requestHistogram, "\th := histo_type(");
                for(i_histo = 0 ; i_histo < 255 ; i_histo++){
                    sprintf(actualHisto, "%ld, ", histogramme[i_histo]);
                    strcat(requestHistogram, actualHisto);
                }
                sprintf(actualHisto, "%ld", histogramme[i_histo]);
                strcat(requestHistogram, actualHisto);
                strcat(requestHistogram, ");\n");
                fputs(requestHistogram, script);

                // Calcul de la MGN et génération requête
                mgn = MGN_from_image(image_nb, nrl, nrh, ncl, nch);
                sprintf(requestMNG, "\tm := %f;\n", mgn);
                fputs(requestMNG, script);

                // Calcul nb pixel contour et génération requête
                nb_pixel_contour = EP_from_image(image_nb, nrl, nrh, ncl, nch);
                sprintf(requestNPC, "\tn := %d;\n", nb_pixel_contour);
                fputs(requestNPC, script);

                // Calcul RGB Rate et génération requête
                rate = rgb_rate_file(actualFilePath);
                sprintf(requestRGBRate, "\tt := taux_type(%f, %f, %f);\n", rate[0], rate[1], rate[2]);
                fputs(requestRGBRate, script);

                // Fin requête
                fputs("\tupdate multimedia\n", script);
                fputs("\tset HISTOGRAMME = h, MOYENNE_NORME_GRADIENT = m, NB_PIXEL_COUNTOUR = n, TAUX_COULEURS = t\n", script);

                sprintf(actualFileName, "\twhere nom = '%s'", fileNameJPG);
                strcat(actualFileName, ";\n");
                fputs(actualFileName, script);
                fputs("\tcommit;\n\n", script);
            }
        }

        fputs("end;", script);


        // Test de fermeture du répertoire
        if(closedir(dir) == -1){
            printf("Erreur fermeture du répertoire.") ;
            exit(-1);
        }

        fclose(script);
    }
    else{
        printf("Erreur ouverture script.\n");
        exit(2);
    }
}

int main(){
    generate_plsql_script("./archive500/ppm/", "./test.sql");
}
