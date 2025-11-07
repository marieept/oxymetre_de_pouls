#include "define.h"
#include "fichiers.h"

/*! ------------------------------------------------------------
  
  \struct param_iir
 
  \brief iir filter
  
  ------------------------------------------------------------*/

  typedef struct{
    float x_avant_acr;
    float y_avant_acr;
    float x_avant_acir;
    float y_avant_acir;
} param_iir;


param_iir* init_iir(param_iir *param);
absorp iir(absorp myAbsorp, param_iir* myIIR);
absorp iirTest(char* filename);