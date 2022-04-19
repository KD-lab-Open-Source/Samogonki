/* this source was algorythmically generated by makeswap.c */

#include "3dstype.h"
#include "swapbyte.h"
void N2ILong3ds(void *native, void *intel){
   *((uint32_t *)(intel)) = *((uint32_t *)(native));
}

void I2NLong3ds(void *intel, void *native){
   *((uint32_t *)(native)) = *((uint32_t *)(intel));
}

void N2IShort3ds(void *native, void *intel){
   *((uint16_t *)(intel)) = *((uint16_t *)(native));
}

void I2NShort3ds(void *intel, void *native){
   *((uint16_t *)(native)) = *((uint16_t *)(intel));
}

void N2IFloat3ds(void *native, void *intel){
   *((float *)(intel)) = *((float *)(native));
}

void I2NFloat3ds(void *intel, void *native){
   *((float *)(native)) = *((float *)(intel));
}

void N2IDouble3ds(void *native, void *intel){
   *((double *)(intel)) = *((double *)(native));
}

void I2NDouble3ds(void *intel, void *native){
   *((double *)(native)) = *((double *)(intel));
}
