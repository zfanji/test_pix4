

#include    "rgbled.h"
#include "main.h"



extern I2C_HandleTypeDef I2c2Handle;


void RGBLED_Init(void)
{
    uint8_t aTxBuffer[5];
	uint8_t ret=0;

    // 蓝色最低亮度
    aTxBuffer[0]=0x01,  //地址 连续开
    aTxBuffer[1]=0x00,  // 蓝
    aTxBuffer[2]=0x00,  // 绿
    aTxBuffer[3]=0x01,  // 红
    aTxBuffer[4]=0x03,  // 开关

	ret= HAL_I2C_Master_Transmit(&I2c2Handle,
     (uint16_t)0xAA, (uint8_t*)aTxBuffer, 5, 10000);
	if(ret==0){
		DebugPrint("TCA62724初始化完成.\r\n");
	}else{
		DebugPrint("TCA62724初始化失败!!!\r\n");
	}
}

void setRGBLED(uint8_t rOn,uint8_t gOn,uint8_t bOn){
	  uint8_t aTxBuffer[5];
		uint8_t ret=0;

    // 蓝色最低亮度
    aTxBuffer[0]=0x01;  //地址 连续开
    aTxBuffer[1]=bOn;  // 蓝
    aTxBuffer[2]=gOn;  // 绿
    aTxBuffer[3]=rOn;  // 红
	if( rOn | gOn | bOn){
			aTxBuffer[4]=0x03;  // 开关
	}else{
			aTxBuffer[4]=0x00;  // 开关
	}
	
	ret= HAL_I2C_Master_Transmit(&I2c2Handle,
		 (uint16_t)0xAA, (uint8_t*)aTxBuffer, 5, 10000);
		
}


