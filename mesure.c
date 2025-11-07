#include "mesure.h"
#include <math.h>
#include "fichiers.h"

//Calculate the peak-to-peak values
float peak2peak(float signal[SIZE]){ //SIZE defined in mesure.h
	float max = signal[0];
	float min = signal[0];

	//Find the maximum and minimum by comparison
	for (int i = 0; i < SIZE; i++){
		if (signal[i] > max){
			max = signal[i];
		}else if (signal[i] < min){
			min = signal[i];
		}
	}

	return max - min; // -min because the minimum is negative
}

//Calculate the heart rate per minute
int bpm(float signal[SIZE]){
	int compteur_de_pics = 0;
	int seuil = 700; // limite value for detecting a peak

	//we find the local maximum by comparison with the neighbors
	for (int i = 1; i < SIZE - 1; i++){
		if (signal[i] > signal[i-1] && signal[i] > signal[i+1] && signal[i] > seuil){
			compteur_de_pics++; //increment the peak counters
		}
	}
	
	float dureeenseconde=(float)SIZE / 500; //sampling rate 500Hz

	int bpm = ((float)compteur_de_pics/dureeenseconde)*60;

	return bpm;
}

//Calculate the RsIR ratio
float formule_RsIR(float ptp_acr, float dcr, float ptp_acir, float dcir){
	float rouge= ptp_acr/dcr;
	float infrarouge= ptp_acir/dcir;
	return rouge/infrarouge;
}

//Convert RsIR ratio into an SpO2 value
int RsIR_en_SpO2(float RsIR){
    //the slope of the line on the value of RsIR
    if (RsIR <= 0.4){
        return 100;

    }else if (RsIR > 0.4 && RsIR <= 1.0){
        return (int)((100-90)/(0.4-0.8) * RsIR + 110); // - 25x +b -> b = 120

    }else if (RsIR > 1.0 && RsIR <= 3.4){
        return (int)((70-50)/(1.4-2) * RsIR + 123.3); // -33.33x+b -> b = 123.3

    }else{
        return 0;
    }
}


//Initializes the measure structure to store the signals
param_mesure* init_mesure(){
	param_mesure* myMes = malloc(sizeof(param_mesure));//allocate memory dynamically

	for (int i = 0; i < SIZE; i++){
		myMes->signal_acr[i] = 0;
		myMes->signal_acir[i] = 0;
	}

	myMes->index = 0;

	return myMes;
}

//Free the memory allocated for mesure
void fin_mesure(param_mesure* myMes){
	free(myMes);
}

//Calculate SpO2 and heart rate with the signals
oxy mesure(absorp myAbsorp, param_mesure * myMes){

	oxy myOxy;

	//Store the new signal values (acr and acir)
	myMes->signal_acr[myMes->index] = myAbsorp.acr;
	myMes->signal_acir[myMes->index] = myAbsorp.acir;

	//Calculate the peak-to-peak values
	float ptp_acr = peak2peak(myMes->signal_acr);
	float ptp_acir = peak2peak(myMes->signal_acir);

	float rsir = formule_RsIR(ptp_acr, myAbsorp.dcr, ptp_acir, myAbsorp.dcir);

	int spo2 = RsIR_en_SpO2(rsir);
	
	int pouls = bpm(myMes->signal_acr);

	//Update the index
	myMes->index = (myMes->index + 1) % SIZE;

	myOxy.spo2 = spo2;
	myOxy.pouls = pouls;

	return myOxy;
}

//Function that will go in the autotest
oxy mesureTest(char* filename){
	int etat = 0;
	absorp myAbsorp, myAbsorpTemp;//myAbsorptemp : temporary structure that takes the values of acr and acir

	oxy myOxy;

	param_mesure* myMes = init_mesure(myMes);

	FILE* myFile = initFichier(filename);

	do{
		myAbsorpTemp = lireFichier(myFile, &etat);

		if (myAbsorpTemp.dcr == 0 && myAbsorpTemp.dcir == 0){
			break;
		}

		myAbsorp = myAbsorpTemp;

    	myOxy = mesure(myAbsorp, myMes);

	}while (etat != EOF);

	fclose(myFile);
	fin_mesure(myMes);

	return myOxy;
}
