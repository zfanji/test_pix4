#include "ms5611.h"


//MS5611片选管脚初始化函数
//输入：void
//输出：void
void MS5611_CS_init(void)
{
	#if 0
		//Define the corresponding structure
    GPIO_InitTypeDef GPIO_InitStructure;
		//Open the appropriate Clock Peripherals
    RCC_AHB1PeriphClockCmd(MS5611_GPIO_Periphera, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = MS5611_CS_PIN;//PD7
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_Init(MS5611_CS_PORT, &GPIO_InitStructure);//初始化
		GPIO_SetBits(MS5611_CS_PORT,MS5611_CS_PIN);
	#endif
}

static void MS5611_WriteCMD( uint8_t WriteCMD )
{
//		SPI1_ReadWriteByte(SPI1, WriteCMD);
}


static void MS5611_ReadData( uint8_t *ReadData )
{
//		*ReadData = SPI1_ReadWriteByte(SPI1, 0x00);
}


static void MS5611_Reset( void )
{
  MS5611_ENABLE;
//  MS5611_WriteCMD(MS5611_RESET);
//  delay_us(10);
  MS5611_DISABLE;
}

static void MS5611_ReadPROM( void )
{
		uint8_t	i=0;
		for(i=0; i<15;i=i+2) 
		{
//	  MS5611_ENABLE;			
//		MS5611_WriteCMD(MS5611_PROM_RD+i);
//		MS5611_ReadData(&(MS5611_PROM_DATA_SHARED.buffer[i+1]));
//		MS5611_ReadData(&(MS5611_PROM_DATA_SHARED.buffer[i]));
//		MS5611_DISABLE;	
//		delay_us(2500);			
		}	
}


static void MS5611_StarADC_Pressure(void)
{
	
		MS5611_ENABLE;
//		MS5611_WriteCMD(MS5611_D1_OSR_4096);
		MS5611_DISABLE;
//		delay_ms(2);	
}

static void MS5611_StarADC_Temperature(void)
{
		MS5611_ENABLE;
	//	MS5611_WriteCMD(MS5611_D2_OSR_4096);
		MS5611_DISABLE;
//		delay_ms(2);		
//
}


static void MS5611_ReadADC_Pressure(void)
{
	//	MS5611_ENABLE;
//		SPI1_ReadWriteByte(SPI1, 0x00);
//		MS5611_NORMAL_DATA_SHARED.buffer1[3]=0;
//		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[2]);
//		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[1]);
//		MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[0]);	
//		MS5611_DISABLE;
//		delay_ms(2);	
}

static void MS5611_ReadADC_Temperature(void)
{
	//	MS5611_ENABLE;
	//	SPI1_ReadWriteByte(SPI1, 0x00);
	//	MS5611_NORMAL_DATA_SHARED.buffer1[7]=0;
	//	MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[6]);
	//	MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[5]);
	//	MS5611_ReadData(&MS5611_NORMAL_DATA_SHARED.buffer1[4]);	
//		MS5611_DISABLE;
//		delay_ms(2);	
}


void MS5611_Init( void )
{
//	MS5611_CS_init();
//	Spi_Init();
  /* Reset */
		MS5611_Reset();

		/* Read PROM */
		MS5611_ReadPROM();
//		delay_ms(3);

		/* D1, D2 Conversion */
		MS5611_StarADC_Pressure();
//		delay_ms(1);
		MS5611_ReadADC_Pressure();

		MS5611_StarADC_Temperature();
//		delay_ms(1);
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
