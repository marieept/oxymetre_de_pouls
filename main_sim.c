#include "define.h"
#include "fichiers.h"
#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "autotests.h"
#include "affichage.h"
#include <stdio.h>



int main(){
    int etat=0;

    absorp myAbsorp;
    oxy myOxy;

    param_fir myFIR;
    init_fir(&myFIR); // init FIR

    param_iir param;
    init_iir(&param); // init IIR

    param_mesure* myMes = init_mesure(); // init mesure
    FILE* myFile = initFichier("log1.dat");
    do{
        myAbsorp = lireFichier(myFile,&etat);

        if (myAbsorp.dcr ==0 && myAbsorp.dcir==0){
            break;
        }

        myAbsorp = fir(myAbsorp,&myFIR);
        myAbsorp = iir(myAbsorp,&param);

        myOxy = mesure(myAbsorp,myMes);

        affichage(myOxy);

        usleep(10000);
    }while(etat != EOF);

    finFichier(myFile);
    fin_mesure(myMes);
    return EXIT_SUCCESS;
}