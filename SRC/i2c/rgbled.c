

#include    "rgbled.h"
#include "main.h"



extern I2C_HandleTypeDef I2c2Handle;


void RGBLED_Init(void)
{
    uint8_t aTxBuffer[5];
	uint8_t ret=0;

    // ��ɫ�������
    aTxBuffer[0]=0x01,  //��ַ ������
    aTxBuffer[1]=0x00,  // ��
    aTxBuffer[2]=0x00,  // ��
    aTxBuffer[3]=0x01,  // ��
    aTxBuffer[4]=0x03,  // ����

	ret= HAL_I2C_Master_Transmit(&I2c2Handle,
     (uint16_t)0xAA, (uint8_t*)aTxBuffer, 5, 10000);
	if(ret==0){
		DebugPrint("TCA62724��ʼ�����.\r\n");
	}else{
		DebugPrint("TCA62724��ʼ��ʧ��!!!\r\n");
	}
}

void setRGBLED(uint8_t rOn,uint8_t gOn,uint8_t bOn){
	  uint8_t aTxBuffer[5];
		uint8_t ret=0;

    // ��ɫ�������
    aTxBuffer[0]=0x01;  //��ַ ������
    aTxBuffer[1]=bOn;  // ��
    aTxBuffer[2]=gOn;  // ��
    aTxBuffer[3]=rOn;  // ��
	if( rOn | gOn | bOn){
			aTxBuffer[4]=0x03;  // ����
	}else{
			aTxBuffer[4]=0x00;  // ����
	}
	
	ret= HAL_I2C_Master_Transmit(&I2c2Handle,
		 (uint16_t)0xAA, (uint8_t*)aTxBuffer, 5, 10000);
		
}


