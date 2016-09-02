/*
 * oled.h
 *
 *  Created on: 2016Äê8ÔÂ30ÈÕ
 *      Author: clevo
 */

#ifndef SRC_OLED_H_
#define SRC_OLED_H_
#include<stdio.h>
#include <unistd.h>
#include "xadcps.h"
#include "xstatus.h"
#include <stdlib.h>
#include "xparameters.h"
#include "xil_cache.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xdmaps.h"
#include "xiicps.h"
#include "xdevcfg.h"
#include "xscutimer.h"
#include "xscuwdt.h"
#include "xparameters.h"


#define MAX_CHAR_POSX 128
#define MAX_CHAR_POSY 64
#define IIC_ADDR_CM	0x3C //oled addr
#define MAX_CHAR_POSX 128
#define MAX_CHAR_POSY 64

int write_command(XIicPs* iic1ps, u8* data, int ByteCount);
int write_data(XIicPs* iic1ps, u8* data, int ByteCount);
void OLED_Refresh_Gram(XIicPs* iic1ps, u8 y, u8 x);
void OLED_Clear(XIicPs* iic0ps);
void OLED_DrawPoint(u8 y, u8 x, u8 t);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_ShowString(XIicPs* iic0ps, u8 x, u8 y, const u8 *p);
void OLED_INIT(XIicPs* iic1ps);
u8 OLED_GRAM[8][128];

#endif /* SRC_OLED_H_ */
