/*
 * copy_buffer.c
 *
 *  Created on: 2016Äê7ÔÂ28ÈÕ
 *      Author: Thinkpad-T440p
 */
#include "copy_buffer.h"



void copy_buffer(unsigned int* input, unsigned int* output)
{
	unsigned int i,j;
	unsigned index;
	for (i = 0; i < FRAME_HEIGHT; i++) {
		for (j = 0; j < FRAME_WIDTH; j++) {
#pragma AP PIPELINE II = 1
			index = i*FRAME_WIDTH + j;
			output[index] = input[index];
		}
	}
}
