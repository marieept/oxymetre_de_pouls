#include "define.h"

/*! ------------------------------------------------------------
  
  \struct param_fir
 
  \brief fir filter
  
  ------------------------------------------------------------*/

  typedef struct{

	float bufferacr[51];
	float bufferacir[51];
	int index;

} param_fir;

extern float FIR_TAPS[51];


float calcul_fir(float* buffer, float* coeffs, int index);
void init_fir(param_fir *myFIR);
absorp fir(absorp myAbsorp, param_fir * myFIR);
absorp firTest(char* filename);