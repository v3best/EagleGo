#include"oled.h"

#define XADC_DEVICE_ID 		XPAR_XADCPS_0_DEVICE_ID
#define printf xil_printf /* Small foot-print printf function */
#define IIC_SLAVE_ADDR		(0x80>>1)//h&t addr
#define TEST_BUFFER_SIZE	2
#define SOM_IIC_SCLK_RATE  100000

/**********************global var***************************/
u8 SendBuffert1[1] = { 0xe3 }; /**< Buffer for Transmitting Data */
u8 SendBufferh[1] = { 0xe5 };
u8 RecvBuffert[TEST_BUFFER_SIZE]; /**< Buffer for Receiving Data */
u8 RecvBufferh[TEST_BUFFER_SIZE];
u32 TempRawData;
u32 VccPdroRawData;
XIicPs iic0ps;
XIicPs iic1ps;
XIicPs_Config* IicPs_0_Config;
XIicPs_Config* IicPs_1_Config;
static XAdcPs XAdcInst; /* XADC driver instance */
float TempData;
float VccPintData;
int init_1_i2c(XIicPs* iic1ps, XIicPs_Config* IicPs_1_Config);
int init_0_i2c(XIicPs* iic0ps, XIicPs_Config* IicPs_0_Config);
int main() {
	int Status;
	int Index;
	XAdcPs_Config *ConfigPtr;
	u16 AdcRawData0; //dust
	u16 AdcRawData1; //light
	u32 TempRawData; //fpga tem
	u32 VccPdroRawData; //fpga vccint
	u16 temp_t = 0;
	u16 temp_h = 0;
	u8 dust[8];
	u8 light[8];
	u8 tem[8];
	u8 hum[8];
	u8 tem_onchip[8];
	u8 VCC_chip[8];
	float ligth_data;
	float dust_data;
	float datat;
	float datah;
	XAdcPs *XAdcInstPtr = &XAdcInst;
	/*******************i2c 0,1 init*********************/
	Status = init_1_i2c(&iic1ps, IicPs_1_Config); //oled i2c config
	if (Status != 0) {
		printf("oled i2c can not config successful\n");
		return -1;
	}
	Status = init_1_i2c(&iic0ps, IicPs_0_Config); //t&h i2c config
	if (Status != 0) {
		printf("t&h i2c can not config successful\n");
		return -1;
	}
	/****************************init buffer******************/
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		RecvBuffert[Index] = 0;
		RecvBufferh[Index] = 0;
	}
	/***********************oled init************************************/
	OLED_INIT(&iic1ps);

	/*******************************xadc init************************/
	ConfigPtr = XAdcPs_LookupConfig(XADC_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr, ConfigPtr->BaseAddress);
	XAdcPs_SetSeqInputMode(XAdcInstPtr, XADCPS_SEQ_MODE_SAFE); //XADCPS_SEQ_CH_AUX06);// XADCPS_SEQ_CH_VCCPINT);
	XAdcPs_SetSeqChEnables(XAdcInstPtr, XADCPS_SEQ_CH_AUX00); // XADCPS_SEQ_CH_VCCPINT);
	XAdcPs_SetSeqChEnables(XAdcInstPtr, XADCPS_SEQ_CH_AUX06); // XADCPS_SEQ_CH_VCCPINT);
			/******************************display sensor data************************/
	while (1) {
		OLED_Clear(&iic1ps);
		/*******************************************************************/
		//acqurie ad data of sensor
		AdcRawData0 = XAdcPs_GetAdcData(XAdcInstPtr, (XADCPS_CH_AUX_MIN));
		dust_data = ((5000 * AdcRawData0 / 65535) - 0.5) / 6.6;
		sprintf((char *) dust, "%.1f", dust_data);
		OLED_ShowString(&iic1ps, 0, 0, (const u8 *) "AQI  :");
		OLED_ShowString(&iic1ps, 48, 0, dust);

		AdcRawData1 = XAdcPs_GetAdcData(XAdcInstPtr, (XADCPS_CH_AUX_MIN + 6));
		ligth_data = (250 * AdcRawData1) / 65535;
		sprintf((char *) light, "%.1f", ligth_data);
		OLED_ShowString(&iic1ps, 0, 16, (const u8 *) "Light:");
		OLED_ShowString(&iic1ps, 48, 16, light);
		OLED_ShowString(&iic1ps, 88, 16, (const u8 *) "lux");
		OLED_Refresh_Gram(&iic1ps, 0, 0);
		/*******************************t&h*****************************/
		XIicPs_MasterSendPolled(&iic0ps, SendBuffert1, 1, IIC_SLAVE_ADDR);
		XIicPs_MasterRecvPolled(&iic0ps, RecvBuffert,
		TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
		temp_t = (RecvBuffert[0] << 8) | RecvBuffert[1];
		temp_t &= 0xfffc;
		datat = temp_t * 175.72 / 65536 - 46.85;
		sprintf((char *) tem, "%.1f", datat);
		OLED_ShowString(&iic1ps, 0, 32, (const u8 *) "Tem  :");
		OLED_ShowString(&iic1ps, 48, 32, tem);
		OLED_ShowString(&iic1ps, 80, 32, (const u8 *) "`C");
		OLED_Refresh_Gram(&iic1ps, 0, 0);

		Status = XIicPs_MasterSendPolled(&iic0ps, SendBufferh, 1,
		IIC_SLAVE_ADDR);

		Status = XIicPs_MasterRecvPolled(&iic0ps, RecvBufferh,
		TEST_BUFFER_SIZE, IIC_SLAVE_ADDR);
		temp_h = ((RecvBufferh[0] << 8) | RecvBufferh[1]);
		temp_h &= 0xfffc;
		datah = temp_h * 125.0 / 65536 - 6;
		sprintf((char *) hum, "%.1f", datah);
		OLED_ShowString(&iic1ps, 0, 48, (const u8 *) "hum  :");
		OLED_ShowString(&iic1ps, 48, 48, hum);
		OLED_ShowString(&iic1ps, 80, 48, (const u8 *) "%RH");
		OLED_Refresh_Gram(&iic1ps, 0, 0);
		usleep(1000000);
		OLED_Clear(&iic1ps);
		//acqurie ad data of FPGA tem
		TempRawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_CH_TEMP);
		TempData = XAdcPs_RawToTemperature(TempRawData);
		sprintf((char *) tem_onchip, "%.1f", TempData);
		OLED_ShowString(&iic1ps, 0, 0, (const u8 *) "Tem_fpga:");
		OLED_ShowString(&iic1ps, 72, 0, tem_onchip);
		OLED_ShowString(&iic1ps, 104, 0, (const u8 *) "`C");
		OLED_Refresh_Gram(&iic1ps, 0, 0);
		//acqurie ad data of FPGA vcc
		VccPdroRawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_CH_VCCINT);
		VccPintData = XAdcPs_RawToVoltage(VccPdroRawData);
		sprintf((char *) VCC_chip, "%.1f", VccPintData);
		OLED_ShowString(&iic1ps, 0, 16, (const u8 *) "Vcc_fpga:");
		OLED_ShowString(&iic1ps, 72, 16, VCC_chip);
		OLED_ShowString(&iic1ps, 96, 16, (const u8 *) "V");
		OLED_Refresh_Gram(&iic1ps, 0, 0);
		usleep(1000000);
	}

	return 0;
}
int init_1_i2c(XIicPs* iic1ps, XIicPs_Config* IicPs_1_Config) {
	int Status;
	IicPs_1_Config = XIicPs_LookupConfig(XPAR_PS7_I2C_1_DEVICE_ID);
	if (IicPs_1_Config == NULL) {
		printf("No XIicPs instance found for ID %d\n\r",
		XPAR_PS7_I2C_1_DEVICE_ID);
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(iic1ps, IicPs_1_Config,
			IicPs_1_Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		printf("init ii2c FAILED XIicps_CfgInitialize\n");
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	Status = XIicPs_SetSClk(iic1ps, SOM_IIC_SCLK_RATE);
	if (Status != XST_SUCCESS) {
		printf("Setting XIicPs clock rate failed for ID %d\n\r", Status);
		return XST_FAILURE;
	}
	return 0;
}
int init_0_i2c(XIicPs* iic0ps, XIicPs_Config* IicPs_0_Config) {
	int Status;
	IicPs_0_Config = XIicPs_LookupConfig(0);
	if (NULL == IicPs_0_Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(iic0ps, IicPs_0_Config,
			IicPs_0_Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	Status = XIicPs_SetSClk(iic0ps, SOM_IIC_SCLK_RATE);
	if (Status != XST_SUCCESS) {
		printf("Setting XIicPs clock rate failed for ID %d\n\r", Status);
		return XST_FAILURE;
	}
	return 0;
}
