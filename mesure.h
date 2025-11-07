#include "define.h"

#define SIZE 1500

typedef struct{
    float signal_acr[SIZE];
    float signal_acir[SIZE];
    int index;
} param_mesure;

oxy mesureTest(char* filename);

param_mesure* init_mesure();
void fin_mesure(param_mesure* myMes);
oxy mesure(absorp myAbsorp, param_mesure * myMes);


int bpm(float signal[SIZE]);
	
