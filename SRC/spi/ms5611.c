#include "ms5611.h"


////定义MS5611的PROM结构体
////定义MS5611的PROM结构体
_MS5611_PROM_DATA_SHARED MS5611_PROM_DATA_SHARED;
_MS5611_NORMAL_DATA_SHARED MS5611_NORMAL_DATA_SHARED;
/*************************************************************************************
*************************************************************************************/

extern SPI_HandleTypeDef Spi1Handle;

void MS5611_CS_ENABLE(void)
{
	
    HAL_GPIO_WritePin(MS5611_CS_PORT, MS5611_CS_PIN, GPIO_PIN_RESET);
}

void MS5611_CS_DISABLE(void)
{
    HAL_GPIO_WritePin(MS5611_CS_PORT, MS5611_CS_PIN, GPIO_PIN_SET);
}

uint8_t MS5611_RW(uint8_t TxData)
{
    uint8_t RxData,ret;
    ret = HAL_SPI_TransmitReceive(&Spi1Handle, &TxData, &RxData, 1, 0x5000);

    return RxData;
}
//MS5611片选管脚初始化函数
//输入：void
//输出：void
void MS5611_CS_init(void)
{
		// IO初始化
    GPIO_InitTypeDef  GPIO_InitStruct;

    //L3GD20  片选IO
    MS5611_CS_CLK_ENABLE();
    GPIO_InitStruct.Pin       = MS5611_CS_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spi片选低有效，上拉保证稳定
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(MS5611_CS_PORT, &GPIO_InitStruct);
    MS5611_CS_DISABLE();

}

static void MS5611_WriteCMD( uint8_t WriteCMD )
{
		MS5611_RW(WriteCMD);
}


static void MS5611_ReadData( uint8_t *ReadData )
{
		*ReadData = MS5611_RW(0x00);
}


static void MS5611_Reset( void )
{
  MS5611_CS_ENABLE();
  MS5611_WriteCMD(MS5611_RESET);
  HAL_Delay(1);  
  MS5611_CS_DISABLE();
}

static void MS5611_ReadPROM( void )
{
		uint8_t	i=0;
		for(i=0; i<15;i=i+2) 
		{
			MS5611_CS_ENABLE();
			MS5611_WriteCMD(MS5611_PROM_RD+i);
			MS5611_ReadData(&(MS5611_PROM_DATA_SHARED.buffer[i+1]));
			MS5611_ReadData(&(MS5611_PROM_DATA_SHARED.buffer[i]));
			MS5611_CS_DISABLE();
			HAL_Delay(3);  ;			
		}	
}


static void MS5611_StarADC_Pressure(void)
{
	
		MS5611_CS_ENABLE();
		MS5611_WriteCMD(MS5611_D1_OSR_4096);
		MS5611_CS_DISABLE();
		HAL_Delay(1);  ;		
}

static void MS5611_StarADC_Temperature(void)
{
		MS5611_CS_ENABLE();
		MS5611_WriteCMD(MS5611_D2_OSR_4096);
		MS5611_CS_DISABLE();
		HAL_Delay(2);	

}


static void MS5611_ReadADC_Pressure(void)
{
		MS5611_CS_ENABLE();
		MS5611_RW(0x00);
		MS5611_NORMAL_DATA_SHARED.buffer1[3]=0;
		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[2]);
		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[1]);
		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[0]);	
		MS5611_CS_DISABLE();
		HAL_Delay(2);	
}

static void MS5611_ReadADC_Temperature(void)
{
		MS5611_CS_ENABLE();
		MS5611_RW(0x00);
		MS5611_NORMAL_DATA_SHARED.buffer1[7]=0;
		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[6]);
		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[5]);
		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[4]);	
		MS5611_CS_DISABLE();
		HAL_Delay(2);	
}


void MS5611_Init( void )
{
  	MS5611_CS_init();
  /* Reset */
		MS5611_Reset();

		/* Read PROM */
		MS5611_ReadPROM();
		HAL_Delay(3);

		/* D1, D2 Conversion */
		MS5611_StarADC_Pressure();
		HAL_Delay(1);
		MS5611_ReadADC_Pressure();

		MS5611_StarADC_Temperature();
		HAL_Delay(1);
		MS5611_ReadADC_Temperature();

//  /* Calculate */
//  MS5611_Calculate();
}

//static void MS5611_Calculate(s32* T, s32 *P)
//{
//		int32_t dT, TEMP, T2 = 0;
//		int64_t OFF, SENS, OFF2 = 0, SENS2 = 0;
//		int32_t lowTEMP, verylowTemp;
//		
//		//////////////////////////////////////////////////////////////////////////
//		//MS5611_PROM_DATA_SHARED是PROM结构体
//		//MS5611_NORMAL_DATA_SHARED是正常的数据结构体

//		dT = (MS5611_NORMAL_DATA_SHARED.MS5611_normal_data.D2_Temperature) 
//					- ((u32)(MS5611_PROM_DATA_SHARED.MS5611_prom_data.MS5611_C5_TREF) << 8);
//	
//		TEMP = 2000 + (((int64_t)dT * MS5611_PROM_DATA_SHARED.MS5611_prom_data.MS5611_C6_TEMPSENS) >> 23);
//	
//		OFF = ((u32)MS5611_PROM_DATA_SHARED.MS5611_prom_data.MS5611_C2_OFF << 16)  
//					+((MS5611_PROM_DATA_SHARED.MS5611_prom_data.MS5611_C4_TCO * (int64_t)dT) >> 7);
//	
//		SENS = ((u32)MS5611_PROM_DATA_SHARED.MS5611_prom_data.MS5611_C1_SENS << 15) 
//					+ ((MS5611_PROM_DATA_SHARED.MS5611_prom_data.MS5611_C3_TCS * (int64_t)dT) >> 8);
//		//
//		*T = TEMP;
//		//////////////////////////////////////////////////////////////////////////
//		//second order temperature compensation
//	if(TEMP < 2000){
//		T2 = (int64_t)((int64_t)dT * (int64_t)dT) >> 31;
//		lowTEMP = TEMP - 2000;
//		lowTEMP *= lowTEMP;
//		OFF2 = (5 * lowTEMP) >> 1;
//		SENS2 = (5 * lowTEMP) >> 2;
//		if(TEMP < -1500){
//			verylowTemp = TEMP + 1500;
//			verylowTemp *= verylowTemp;
//			OFF2 = OFF2 + 7 * verylowTemp;
//			SENS2 = SENS2 + ((11 * verylowTemp) >> 1);
//		}
//		//
//		OFF = OFF - OFF2;
//		SENS = SENS - SENS2;
//		*T = TEMP - T2;
//	}
//	//////////////////////////////////////////////////////////////////////////
//	*P = ((((int64_t)MS5611_NORMAL_DATA_SHARED.MS5611_normal_data.D1_Pressure * SENS) >> 21) - OFF) >> 15;  

//}
