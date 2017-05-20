


#include "MPU6000.h"
#include "stm32f4xx.h"
#include "main.h"



MPU_report  MPU_report1;
extern SPI_HandleTypeDef Spi1Handle;
uint16_t    MPU_RD_CNT;


void MPU6000_Init(void)
{
    // IO��ʼ��
    GPIO_InitTypeDef  GPIO_InitStruct;

    //MPU6000  ƬѡIO
    MPU_CS_CLK_ENABLE();
    GPIO_InitStruct.Pin       = MPU_CS_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spiƬѡ����Ч��������֤�ȶ�
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(MPU_CS_PORT, &GPIO_InitStruct);
    MPU6000_CS_DISABLE();

    //MPU6000 DRDY���� --�ⲿ�ж� �����ش���
    MPU_DRDY_CLK_ENABLE();
    GPIO_InitStruct.Pin       = MPU_DRDY_PIN;       // GPIO15 ����н��� ��ע��
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING; // �ж�ģʽ �������ж�
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spiƬѡ����Ч��������֤�ȶ�
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(MPU_DRDY_PORT, &GPIO_InitStruct);

    //�ⲿ�жϳ�ʼ��
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0); // �����ⲿ�����ж�
    MPU6000_INT_ENABLE();


    //�Ĵ�����ʼ��
    MPU6000_SET(MPUREG_PWR_MGMT_1,0x80);        // ��λ
    HAL_Delay(10);                              // �ȴ�10ms
    MPU6000_SET(MPUREG_PWR_MGMT_1,0x03);    //����ʱ��ԴΪ�����ǵ�Z��
    HAL_Delay(1);
    MPU6000_SET(MPUREG_USER_CTRL,0x10);     //ʹ��SPI���� ����I2C //�ر���FIFO
    HAL_Delay(1);
    MPU6000_SET(MPUREG_SMPLRT_DIV,0x00);    //������Ƶ�� ����ֵ0x00(1kHz);
    //MPU6000_SET(MPUREG_SMPLRT_DIV,0xF9);    //������Ƶ�� ����ֵ0xFA(4Hz);
    HAL_Delay(1);
    MPU6000_SET(MPUREG_CONFIG,0x03);        //��ͨ�˲�Ƶ�� 0x03��Ϊ42Hz
    HAL_Delay(1);
    MPU6000_SET(MPUREG_GYRO_CONFIG,0x18);   //������������� +-2000��/�� �����Լ�
    HAL_Delay(1);
    MPU6000_SET(MPUREG_ACCEL_CONFIG,0x10);  //���ٶȼ��������  +-8g �����Լ�
    HAL_Delay(1);
    MPU6000_SET(MPUREG_INT_ENABLE, 0x01);   //�������жϣ�������������׼�����Ժ�ͳ����ж�
    HAL_Delay(1);
    MPU6000_SET(MPUREG_INT_PIN_CFG, 0x30);  //�����ն�ģʽ���ߵ�ƽ���������Ĵ����������ж�
    HAL_Delay(1);

    DebugPrint("MPU6000��ʼ�����!\r\n");

}



void MPU6000_INT_ENABLE(void)
{
   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void MPU6000_INT_DISABLE(void)
{
   HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}


uint8_t MPU6000_RW(uint8_t TxData)
{
    uint8_t RxData;
    HAL_SPI_TransmitReceive(&Spi1Handle, &TxData, &RxData, 1, 0x5000);
    return RxData;
}

void MPU6000_SET(uint8_t setAddr,uint8_t setData)
{
   MPU6000_CS_ENABLE();
   MPU6000_RW(setAddr);	//д��ַ
   MPU6000_RW(setData); // д����
   MPU6000_CS_DISABLE();
}





void MPU6000_CS_ENABLE(void)
{
    HAL_GPIO_WritePin(MPU_CS_PORT, MPU_CS_PIN, GPIO_PIN_RESET);
}

void MPU6000_CS_DISABLE(void)
{
    HAL_GPIO_WritePin(MPU_CS_PORT, MPU_CS_PIN, GPIO_PIN_SET);
}






void SPI_MPU_Read(uint8_t *Buffer, uint8_t ReadAddr,uint8_t NumByteToRead)
{
 	uint8_t i;
    MPU6000_CS_ENABLE();
    MPU6000_RW(ReadAddr|=0x80);         //

    for(i=0;i<NumByteToRead;i++)
	{
        Buffer[i]=MPU6000_RW(0xFF);
    }
	MPU6000_CS_DISABLE();
}




void MPU6000_RAW_READY(MPU_report *pMPU_report)
{
   uint8_t MPU_buff[14]={0};

   SPI_MPU_Read(MPU_buff,MPUREG_ACCEL_XOUT_H,14);

   pMPU_report->timestamp = HAL_GetTick();
   pMPU_report->accel_x_raw =      (((int16_t)MPU_buff[0]) << 8) | MPU_buff[1];
   pMPU_report->accel_y_raw =      (((int16_t)MPU_buff[2]) << 8) | MPU_buff[3];
   pMPU_report->accel_z_raw =      (((int16_t)MPU_buff[4]) << 8) | MPU_buff[5];
   pMPU_report->temperature_raw =  (((int16_t)MPU_buff[6]) << 8) | MPU_buff[7];
   pMPU_report->gyro_x_raw =       (((int16_t)MPU_buff[8]) << 8) | MPU_buff[9];
   pMPU_report->gyro_y_raw =       (((int16_t)MPU_buff[10]) << 8) | MPU_buff[11];
   pMPU_report->gyro_z_raw =       (((int16_t)MPU_buff[12]) << 8) | MPU_buff[13];
   pMPU_report->flag = 1;
}


void EXTI15_10_IRQHandler(void)
{


    if(__HAL_GPIO_EXTI_GET_IT(MPU_DRDY_PIN) != RESET)
      {
        MPU6000_RAW_READY(&MPU_report1);
        MPU_RD_CNT++;
        __HAL_GPIO_EXTI_CLEAR_IT(MPU_DRDY_PIN);
      }

}



