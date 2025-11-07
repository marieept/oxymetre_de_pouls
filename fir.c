#include <stdio.h>
#include <stdlib.h>

#include "fichiers.h"
#include "fir.h"

//Filter coefficients
float FIR_TAPS[51]={
    1.4774946e-019,1.6465231e-004,3.8503956e-004,7.0848037e-004,
    1.1840522e-003,1.8598621e-003,2.7802151e-003,3.9828263e-003,
    5.4962252e-003,7.3374938e-003,9.5104679e-003,1.2004510e-002,
    1.4793934e-002,1.7838135e-002,2.1082435e-002,2.4459630e-002,
    2.7892178e-002,3.1294938e-002,3.4578348e-002,3.7651889e-002,
    4.0427695e-002,4.2824111e-002,4.4769071e-002,4.6203098e-002,
    4.7081811e-002,4.7377805e-002,4.7081811e-002,4.6203098e-002,
    4.4769071e-002,4.2824111e-002,4.0427695e-002,3.7651889e-002,
    3.4578348e-002,3.1294938e-002,2.7892178e-002,2.4459630e-002,
    2.1082435e-002,1.7838135e-002,1.4793934e-002,1.2004510e-002,
    9.5104679e-003,7.3374938e-003,5.4962252e-003,3.9828263e-003,
    2.7802151e-003,1.8598621e-003,1.1840522e-003,7.0848037e-004,
    3.8503956e-004,1.6465231e-004,1.4774946e-019
};

//Function to apply the FIR filter
float calcul_fir (float* buffer, float* coeffs, int index){
	float result = 0.0;

	//Apply FIR filter
	for (int i =0; i<51; i++){
		int buffer_index = (index -i +51) %51; //the +51 is used to go through all the values without going negative
		result += coeffs[i] * buffer[buffer_index];
	}

	return result;
}

//Initialize FIR filter 
void init_fir(param_fir *myFIR){
	for (int i=0; i<51; i++){
		myFIR->bufferacr[i]=0.0;
		myFIR->bufferacir[i]=0.0;
	}

	myFIR->index =0;

}

//Apply FIR filter
absorp fir(absorp myAbsorp, param_fir * myFIR){
	
	//Assign the values of acr and acir
	myFIR->bufferacr[myFIR->index] = myAbsorp.acr;
    myFIR->bufferacir[myFIR->index] = myAbsorp.acir;

	//Apply the filter to acr and acir values
	myAbsorp.acr = calcul_fir(myFIR->bufferacr, FIR_TAPS, myFIR->index);
	myAbsorp.acir = calcul_fir(myFIR->bufferacir, FIR_TAPS, myFIR->index);

	//Update index value
	myFIR->index = (myFIR->index +1) %51;//we use modulo 51 to return to 0 afterwards

	return myAbsorp;
}

//Function that will go in the autotest
absorp firTest(char* filename){
	
	int etat=0;
    absorp myAbsorp, myAbsorptemp;
	//myAbsorptemp : temporary structure that takes the values of acr and acir 


    param_fir myFIR;
	init_fir(&myFIR); // init FIR

    FILE* myFile = initFichier(filename);

	do{
		myAbsorptemp = lireFichier(myFile, &etat);

		if (myAbsorptemp.dcr ==0 && myAbsorptemp.dcir==0){ // the file knows it has reached the end only when it is at the end
			break;//we don't consider the last line

		}

		myAbsorp = myAbsorptemp; //structure assignment
		myAbsorp = fir(myAbsorp, &myFIR);//we apply the FIR filter

	}while (etat !=EOF);

	fclose(myFile);

	return myAbsorp;

}
