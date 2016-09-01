#ifndef _CONV2_H_
#define _CONV2_H_

#define conv2_imX 28 //the picture weight
#define conv2_imY 28 //the picture height
#define conv2_K 32 //number of kernel
#define conv2_C 32 //channel
#define conv2_X 5 //kernel weight
#define conv2_Y 5 //kernel height
#define conv2_I (conv2_imX-conv2_X+1) //out weight
#define conv2_J (conv2_imY-conv2_Y+1) //out height
#include "mult_core.h"

float* conv2(float im[conv2_C][conv2_imX][conv2_imY],float rst[conv2_K][conv2_I][conv2_J]);

#endif
