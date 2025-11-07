#include "iir.h"
#include "fichiers.h"

//Initialize IIR filter
param_iir* init_iir(param_iir *myIIR){

	//everything is initialized to 0
	myIIR->x_avant_acr=0;
	myIIR->y_avant_acr=0;
	myIIR->x_avant_acir=0;
	myIIR->y_avant_acir=0;

	return myIIR;
}

//Apply IIR filter
absorp iir(absorp myAbsorp, param_iir* myIIR){
	
	// Calculate for acr
	myIIR->y_avant_acr = myAbsorp.acr - myIIR->x_avant_acr + 0.992 * myIIR->y_avant_acr;//apply the IIR filter 

	myIIR->x_avant_acr = myAbsorp.acr;//update the previous value for acr
	
	// Calculate for acir
	myIIR->y_avant_acir = myAbsorp.acir - myIIR->x_avant_acir + 0.992 * myIIR->y_avant_acir;//apply the IIR filter

	myIIR->x_avant_acir = myAbsorp.acir;//update the previous value for acir

	//Put the filter values in the structure
	myAbsorp.acr = myIIR->y_avant_acr;
	myAbsorp.acir = myIIR->y_avant_acir;

	return myAbsorp;
}

//Function that will go in the autotest
absorp iirTest(char* filename){
	int etat=0;
    absorp myAbsorp, myAbsorptemp;
	//myAbsorptemp : temporary structure that takes the values of acr and acir

    param_iir myIIR;
	init_iir(&myIIR); // init IIR

    FILE* myFile = initFichier(filename);

	do{
		myAbsorptemp = lireFichier(myFile, &etat);

		if (myAbsorptemp.dcr ==0 && myAbsorptemp.dcir==0){ // the file knows it has reached the end only when it is at the end
			break; //we don't consider the last line
		}

		myAbsorp = myAbsorptemp; //structure assignment
		myAbsorp = iir(myAbsorp, &myIIR);//we apply the FIR filter

	}while (etat !=EOF);

	fclose(myFile);
	return myAbsorp;
}

