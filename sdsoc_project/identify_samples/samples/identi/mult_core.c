#include <stdio.h>
#include <stdlib.h>

#include "mult_core.h"

void mmult_kernel(float in_A[A_NROWS][A_NCOLS],
                  float in_B[B_NCOLS][B_NCOLS],
                  float out_C[28*28])
{
#pragma HLS INLINE self
#pragma HLS array_partition variable=in_A complete dim=2
#pragma HLS array_partition variable=in_B complete dim=2

  int i,j,x,y;

  for (i = 0; i < 28; i++) {
    for (j = 0; j < 28; j++) {
#pragma HLS PIPELINE
      float result = 0;
      for (x = 0; x < A_NCOLS; x++) {
        // multiply accumulate broken into individual operators
        // so that AutoESL can infer two FP operators
#pragma HLS UNROLL
    	for(y = 0; y < A_NCOLS; y++) {
    		float product_term = in_A[x][y] * in_B[x+i][y+j];
    		result += product_term;
    	}
      }
      out_C[i*28+j] = result;
    }
  }
}

void mmult(float in_A[A_NROWS][A_NCOLS],
        float in_B[B_NCOLS][B_NCOLS],
           float out_C[28*28])
{
  int i, j;
  float a_buf[A_NROWS][A_NCOLS];
  float b_buf[B_NCOLS][B_NCOLS];

  // Transfer matrix A from multi-buffer into local RAM
  for(i=0; i<A_NROWS; i++) {
    for(j=0; j<A_NCOLS; j++) {
#pragma HLS PIPELINE
      a_buf[i][j] = in_A[i][j];
    }
  }

  // Transfer matrix B from multi-buffer into local RAM
  for(i=0; i<B_NCOLS; i++) {
    for(j=0; j<B_NCOLS; j++) {
#pragma HLS PIPELINE
      b_buf[i][j] = in_B[i][j];
    }
  }

  // Matrix multiply call
  mmult_kernel(a_buf, b_buf, out_C);
}
