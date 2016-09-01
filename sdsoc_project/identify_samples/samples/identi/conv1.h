#include "mult_core.h"

#define conv1_imX 32 //the picture weight
#define conv1_imY 32 //the picture height
#define conv1_K 32 //number of kernel
#define conv1_C 3 //channel
#define conv1_X 5 //kernel weight
#define conv1_Y 5 //kernel height
#define conv1_I conv1_imX-conv1_X+1 //out weight
#define conv1_J conv1_imY-conv1_Y+1 //out height

void conv1(float im[conv1_C][conv1_imX][conv1_imY],float rst[conv1_K][conv1_I][conv1_J]);
