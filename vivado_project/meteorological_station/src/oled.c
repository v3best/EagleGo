#include"oled.h"
#include "font.h"
u8 command[] = { 0xae, 0xd5, 0x80, 0xa8, 0x3f, 0xd3, 0x00, 0x40, 0x8d, 0x14,
		0x20, 0x02, 0xa1, 0xc0, 0xda, 0x12, 0x81, 0xef, 0xd9, 0xf1, 0xdb, 0x30,
		0xa4, 0xa6, 0xaf };
int write_command(XIicPs* iic1ps, u8* data, int ByteCount) {

	int Status;
	int i;
	u8* WriteBuffer = (u8*) malloc((ByteCount + 1) * sizeof(u8));
	WriteBuffer[0] = 0x00;
	for (i = 0; i < ByteCount; i++) {

		WriteBuffer[i + 1] = data[i];
	}
	while (XIicPs_BusIsBusy(iic1ps)) {
		/* NOP */
	}
	/** Send the buffer using the IIC and check for errors.*/

	Status = XIicPs_MasterSendPolled(iic1ps, WriteBuffer, ByteCount + 1,
	IIC_ADDR_CM);
	free(WriteBuffer);
	if (Status != XST_SUCCESS) {
		printf("XIicPs_MasterSendPolled error!\n\r");

	}

	return 0;
}

int write_data(XIicPs* iic1ps, u8* data, int ByteCount) {

	int Status;
	int i;
	u8* WriteBuffer = (u8*) malloc((ByteCount + 1) * sizeof(u8));
	WriteBuffer[0] = 0x40;
	for (i = 0; i < ByteCount; i++) {

		WriteBuffer[i + 1] = data[i];
	}
	while (XIicPs_BusIsBusy(iic1ps)) {
		/* NOP */
	}
	/** Send the buffer using the IIC and check for errors.*/

	Status = XIicPs_MasterSendPolled(iic1ps, WriteBuffer, ByteCount + 1,
	IIC_ADDR_CM);
	free(WriteBuffer);
	if (Status != XST_SUCCESS) {
		printf("XIicPs_MasterSendPolled error!\n\r");
		return XST_FAILURE;
	}

	return 0;
}

void OLED_Refresh_Gram(XIicPs* iic1ps, u8 y, u8 x) {
	u8 i = 0, n = 0;
	u8 refresh = { 0xb0 };
	u8 refresh1 = { 0x02 };
	u8 refresh2 = { 0x10 };

	for (i = 0; i < 8; i++) {

		write_command(iic1ps, &refresh, 1);
		write_command(iic1ps, &refresh1, 1);
		write_command(iic1ps, &refresh2, 1);
		refresh = refresh + 1;
		for (n = 0; n < 128; n++)
			write_data(iic1ps, &OLED_GRAM[i][n], 1);
	}
}

void OLED_Clear(XIicPs* iic0ps) {
	u8 i, n;
	for (i = 0; i < 8; i++) {
		for (n = 0; n < 128; n++)
			OLED_GRAM[i][n] = 0X00;
	}
	OLED_Refresh_Gram(iic0ps, 0, 0);
}

void OLED_DrawPoint(u8 y, u8 x, u8 t) {
	u8 pos, by, temp = 0;
	if (x > 127 || y > 63)
		return;
	pos = 7 - y / 8;
	//pos = y/8;
	by = y % 8;
	temp = 1 << (7 - by);
	//temp = 1 << by;
	if (t)
		OLED_GRAM[pos][x] |= temp;
	else
		OLED_GRAM[pos][x] &= ~temp;
}

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode) {
	u8 temp, t, t1;
	u8 y0 = y;
	chr = chr - ' ';
	for (t = 0; t < size; t++) {
		if (size == 12)
			temp = asc2_1206[chr][t];
		else
			temp = asc2_1608[chr][t];
		for (t1 = 0; t1 < 8; t1++) {
			if (temp & 0x80)
				OLED_DrawPoint(y, x, mode);
			else
				OLED_DrawPoint(y, x, !mode);
			temp = temp << 1;
			y++;
			if ((y - y0) == size) {
				y = y0;
				x++;
				break;
			}
		}
	}
}

void OLED_ShowString(XIicPs* iic0ps, u8 x, u8 y, const u8 *p) {

	while (*p != '\0') {
		if (x > MAX_CHAR_POSX) {
			x = 0;
			y += 16;
		}
		if (y > MAX_CHAR_POSY) {
			y = x = 0;
			OLED_Clear(iic0ps);

		}
		OLED_ShowChar(x, y, *p, 16, 1);
		x += 8;
		p++;
	}
}

void OLED_INIT(XIicPs* iic1ps) {
	u8 i = 0;
	for (i = 0; i < sizeof(command); i++) {
		write_command(iic1ps, &command[i], 1);
	}
}
