

#include    "rgbled.h"



extern I2C_HandleTypeDef I2c2Handle;


void RGBLED_Init(void)
{
    uint8_t aTxBuffer[5];

    // ��ɫ�������
    aTxBuffer[0]=0x01,  //��ַ ������
    aTxBuffer[1]=0x01,  // ��
    aTxBuffer[2]=0x00,  // ��
    aTxBuffer[3]=0x00,  // ��
    aTxBuffer[4]=0x03,  // ����

    HAL_I2C_Master_Transmit(&I2c2Handle,
     (uint16_t)0xAA, (uint8_t*)aTxBuffer, 5, 10000);
}


