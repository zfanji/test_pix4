/********************************************************************************
  * @file    ms5611.h 
  * @author  Huangzhibin
  * @version V1.0.0
  * @date    10-12-2015
  * @brief   Header for ms5611.c module
*******************************************************************************/ 
#ifndef _MS5611_H_
#define _MS5611_H_
#include "stm32f4xx_hal_conf.h"
#include "stdint.h"
#include "spi.h"
//#include "delay.h"
//**********************************************************************************//
#define MS5611_ADC            ((uint8_t)0x00)  // ADC Read
#define MS5611_RESET          ((uint8_t)0x1E)  // RESET

#define MS5611_D1_OSR_256     ((uint8_t)0x40)  // 3 bytes
#define MS5611_D1_OSR_512     ((uint8_t)0x42)  // 3 bytes
#define MS5611_D1_OSR_1024    ((uint8_t)0x44)  // 3 bytes
#define MS5611_D1_OSR_2048    ((uint8_t)0x46)  // 3 bytes
#define MS5611_D1_OSR_4096    ((uint8_t)0x48)  // 3 bytes

#define MS5611_D2_OSR_256     ((uint8_t)0x50)  // 3 bytes
#define MS5611_D2_OSR_512     ((uint8_t)0x52)  // 3 bytes
#define MS5611_D2_OSR_1024    ((uint8_t)0x54)  // 3 bytes
#define MS5611_D2_OSR_2048    ((uint8_t)0x56)  // 3 bytes
#define MS5611_D2_OSR_4096    ((uint8_t)0x58)  // 3 bytes

#define MS5611_ADC_D1         ((u8)0x48)	//选择4096
#define MS5611_ADC_D2         ((u8)0x58)	//选择4096

#define MS5611_PROM_RD				((uint8_t)0xA0)  // 2 bytes
#define MS5611_PROM_COEFF_1		((uint8_t)0xA2)  // 2 bytes
#define MS5611_PROM_COEFF_2		((uint8_t)0xA4)  // 2 bytes
#define MS5611_PROM_COEFF_3		((uint8_t)0xA6)  // 2 bytes
#define MS5611_PROM_COEFF_4		((uint8_t)0xA8)  // 2 bytes
#define MS5611_PROM_COEFF_5		((uint8_t)0xAA)  // 2 bytes
#define MS5611_PROM_COEFF_6		((uint8_t)0xAC)  // 2 bytes
#define MS5611_PROM_CRC				((uint8_t)0xAE)  // 2 bytes
/*************************************************************************************
16 bit reserved for manufacturer			0xA0			
MS5611_PROM_COEFF_1										0xA2			//#define MS5611_PROM_COEFF_1   ((u8)0xA2)  // 2 bytes
MS5611_PROM_COEFF_2										0xA4			//#define MS5611_PROM_COEFF_2   ((u8)0xA4)  // 2 bytes
MS5611_PROM_COEFF_3										0xA6			//#define MS5611_PROM_COEFF_3   ((u8)0xA6)  // 2 bytes
MS5611_PROM_COEFF_4										0xA8			//#define MS5611_PROM_COEFF_4   ((u8)0xA8)  // 2 bytes
MS5611_PROM_COEFF_5										0xAA			//#define MS5611_PROM_COEFF_5   ((u8)0xAA)  // 2 bytes
MS5611_PROM_COEFF_6										0xAC			//#define MS5611_PROM_COEFF_6   ((u8)0xAC)  // 2 bytes
MS5611_PROM_CRC												0xAE			//#define MS5611_CRC   					((u8)0xAE)  // 2 bytes

*************************************************************************************/
#define MS5611_RespFreq_256   ((u16)1650) // 0.48 - 0.54 - 0.60 - 1650Hz		// 1010~1000  UP 	100m 	DOWN 	10.5mbar		
#define MS5611_RespFreq_512   ((u16)850)  // 0.95 - 1.06 - 1.17 - 850Hz			// 1000~900   UP	100m 	DOWN	11.2mbar		
#define MS5611_RespFreq_1024  ((u16)430)  // 1.88 - 2.08 - 2.28 - 430Hz			// 900~800    UP	100m 	DOWN	12.2mbar
#define MS5611_RespFreq_2048  ((u16)220)  // 3.72 - 4.13 - 4.54 - 220Hz			// 800~700    UP	100m 	DOWN	13.3mbar
#define MS5611_RespFreq_4096  ((u16)110)  // 7.40 - 8.22 - 9.04 - 110Hz			// 700~600    UP	100m 	DOWN	15mbar
//**********************************************************************************//

/*************************************************************************************
//MS5611的PROM出厂校准数据结构体
*************************************************************************************/
typedef struct _MS5611_prom_data
{
    uint16_t    manufacturer;      					// 16预留给制造商
    uint16_t    MS5611_C1_SENS;      				// 压力灵敏度
    uint16_t    MS5611_C2_OFF;      				// 压力抵消
    uint16_t    MS5611_C3_TCS ;      				// 温度压力灵敏度系数	
    uint16_t    MS5611_C4_TCO ;      				// 温度系数的压力抵消	
    uint16_t    MS5611_C5_TREF;      				// 参考温度
    uint16_t    MS5611_C6_TEMPSENS ;      	// 温度系数的温度	
    uint16_t    MS5611_CRC ;      					// CRC校验，只有低4位有效数据	
}_MS5611_PROM_DATA,*P_MS5611_PROM_DATA;
//这里是小端模式，在数据转换的时候，位置要注意，例如：MS5611_C1_SENS的高位是buffer[3]，低位是buffer[2];
typedef   union _MS5611_prom_data_shared
{
    _MS5611_PROM_DATA MS5611_prom_data;
    uint8_t           buffer[sizeof(_MS5611_PROM_DATA)];
}_MS5611_PROM_DATA_SHARED;

/*************************************************************************************
*************************************************************************************/

/*************************************************************************************
//MS5611的数字输出值
**************************************************************************************/
typedef struct _MS5611_normal_data
{
    uint32_t    D1_Pressure;      					// D1为数字压力值
    uint32_t    D2_Temperature;      				// D2为测量温度值
}_MS5611_NORMAL_DATA,*P_MS5611_NORMAL_DATA;

typedef   union _MS5611_normal_data_shared
{
    _MS5611_NORMAL_DATA MS5611_normal_data;
    uint8_t           buffer1[sizeof(_MS5611_NORMAL_DATA)];
}_MS5611_NORMAL_DATA_SHARED;

/*************************************************************************************
*************************************************************************************/




//***********************************//
/*****MPU6000	CS	Pin	definition*****/
#if 1
#define MS5611_CS_PIN                      GPIO_PIN_7
#define MS5611_CS_PORT                     GPIOD
#define MS5611_CS_CLK_ENABLE()             __GPIOD_CLK_ENABLE()
#endif
//***********************************//
void MS5611_CS_init(void);
void MS5611_Init( void );
static void MS5611_WriteCMD( uint8_t WriteCMD );
static void MS5611_ReadData( uint8_t *ReadData );
static void MS5611_Reset( void );
static void MS5611_ReadPROM( void );
static void MS5611_StarADC_Pressure(void);
static void MS5611_StarADC_Temperature(void);
static void MS5611_ReadADC_Pressure(void);
static void MS5611_ReadADC_Temperature(void);
void getMs5611Data( void );
//static void MS5611_Calculate( void );

void MS5611_SPIx_ReadADC(void);

#endif /* _MS5611_H_ */

/******************* (C) COPYRIGHT 2015 Huangzhibin*****END OF FILE****/
