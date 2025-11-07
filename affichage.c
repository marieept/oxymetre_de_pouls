#include "affichage.h"
#include "define.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Write the SpO2 and heart rate values in data.txt
void affichage(oxy myOxy){
    if ( access( ".verrouData", F_OK ) != -1){ // if the lock file exists
        return; 
    }else{

    //create and open the lock file to respect the mutual exclusion
    FILE *verrou = fopen(".verrouData", "w");
        if (verrou == NULL){//if the lock file can't be found
            printf("Impossible d'ouvrir le fichier verrou\n");
            return;
        }

    // we open data.txt in write mode
    FILE *data = fopen("data.txt", "w");

    if (data == NULL){ //if data can't be opened
        printf("Impossible d'ouvrir le fichier data\n");
        remove(".verrouData");
        return;
    }

    // Write SpO2 and heart rate values into data.txt
    fprintf(data, "%d\n%d", myOxy.spo2, myOxy.pouls);

    fclose(data);
    fclose(verrou);

    //remove the lock file
    remove(".verrouData");

    }	
}

