#ifndef _MULT_CORE_H_
#define _MULT_CORE_H_

#define A_NROWS 5
#define A_NCOLS A_NROWS
#define B_NCOLS 32
#define B_NROWS A_NCOLS
//james delete the comment below
#pragma SDS data access_pattern(in_A:SEQUENTIAL, in_B:SEQUENTIAL, out_C:SEQUENTIAL)

void mmult(float in_A[A_NROWS][A_NCOLS],
        float in_B[B_NCOLS][B_NCOLS],
           float out_C[28*28]);
#endif /* _MULT_CORE_H_ */

