#include "lsm303d.h"
#include "MPU6000.h"
#include "stm32f4xx.h"
#include "main.h"



LSM303D_ACC_report  LSM303D_ACC_report1;
LSM303D_MAG_report  LSM303D_MAG_report1;

extern SPI_HandleTypeDef Spi1Handle;

uint16_t    LSM303D_RD_CNT;


void LSM303D_Init(void)
{
    // IO��ʼ��
    GPIO_InitTypeDef  GPIO_InitStruct;

    //LSM303D  ƬѡIO
    LSM303D_CS_CLK_ENABLE();
    GPIO_InitStruct.Pin       = LSM303D_CS_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spiƬѡ����Ч��������֤�ȶ�
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LSM303D_CS_PORT, &GPIO_InitStruct);
    LSM303D_CS_DISABLE();

    //LSM303D DRDY���� --�ⲿ�ж� �����ش���
    LSM303D_ACC_DRDY_CLK_ENABLE();
    GPIO_InitStruct.Pin       = LSM303D_ACC_DRDY_PIN;       // GPIO4 ����н��� ��ע��
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING; // �ж�ģʽ �������ж�
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spiƬѡ����Ч��������֤�ȶ�
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LSM303D_ACC_DRDY_PORT, &GPIO_InitStruct);

    LSM303D_MAG_DRDY_CLK_ENABLE();
    GPIO_InitStruct.Pin       = LSM303D_MAG_DRDY_PIN;       // GPIO1 ����н��� ��ע��
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING; // �ж�ģʽ �������ж�
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spiƬѡ����Ч��������֤�ȶ�
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LSM303D_MAG_DRDY_PORT, &GPIO_InitStruct);

    //�ⲿ�жϳ�ʼ��  acc_int
    HAL_NVIC_SetPriority(EXTI4_IRQn, 4, 0); // �����ⲿ�����ж�
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);         // �����ⲿ�����ж�

    // mag_int
    HAL_NVIC_SetPriority(EXTI1_IRQn, 4, 0); // �����ⲿ�����ж�
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);         // �����ⲿ�����ж�


    // ��ʼ��LSM303D��Ҫʹ��SPI1  ���ѱ�MPU6000ռ��
    // Ϊ�˷���ײ �����Ҫ�ȹص�SPI1���Ѿ���ɳ�ʼ����оƬ�� RD�ж�
    MPU6000_INT_DISABLE();

    // ��ʼ��ʼ��LSM303D

    LSM303D_SET(LSM303D_REG_CTRL0, 0x80);    // ��λ
    HAL_Delay(10); // 10MS  // Ϲ�µ� δ���ֲ�
    LSM303D_SET(LSM303D_REG_CTRL0, 0x00);   // �ص���λ����ʹ��FIFO, ��ʹ�ø�ͨ�˲���
    HAL_Delay(1);
            //  acc800HZ  ������ACCֱ��֮ǰ�����ݱ���ȡ  ʹ�ܼ��ٶȼ�
    LSM303D_SET(LSM303D_REG_CTRL1,
    REG1_RATE_800HZ_A | REG1_BDU_UPDATE | REG1_Z_ENABLE_A | REG1_Y_ENABLE_A | REG1_X_ENABLE_A);
    HAL_Delay(1);

    // ACC������˲�50Hz  +-8g  �ر��Բ�   4��SPI
    // ������������˲�ʹ�� �ǳ���Ҫ
    LSM303D_SET(LSM303D_REG_CTRL2,REG2_AA_FILTER_BW_50HZ_A | REG2_FULL_SCALE_8G_A );
    HAL_Delay(1);


    //ʹ���¶ȴ���  �Ÿ߷ֱ��� ����Ƶ�� 100Hz   ���жϼĴ�������
    LSM303D_SET(LSM303D_REG_CTRL5,
    REG5_ENABLE_T | REG5_RES_HIGH_M | REG5_RATE_100HZ_M);
    HAL_Delay(1);

    // ʹ��MAG  ����ת��ģʽ   ʣ�๦�ܾ�����ر�
    LSM303D_SET(LSM303D_REG_CTRL6,  REG6_FULL_SCALE_2GA_M);
    HAL_Delay(1);

    // MAG range 2
    LSM303D_SET(LSM303D_REG_CTRL7,  REG7_CONT_MODE_M);
    HAL_Delay(1);

    // �ߵ�ƽ�ж�
    LSM303D_SET(LSM303D_INT_CTRL_M, 0x08);
    HAL_Delay(1);

    // ���ж�
    LSM303D_SET(LSM303D_REG_CTRL3, 0x04);  // DRDY on ACCEL on INT1
    HAL_Delay(1);
    LSM303D_SET(LSM303D_REG_CTRL4, 0x04);  // DRDY on MAG on INT2
    HAL_Delay(1);


    MPU6000_INT_ENABLE();       // ������� ���¿������ص����ж�

    DebugPrint("LSM303D��ʼ�����!\r\n");

}



void LSM303D_CS_ENABLE(void)
{
    HAL_GPIO_WritePin(LSM303D_CS_PORT, LSM303D_CS_PIN, GPIO_PIN_RESET);
}

void LSM303D_CS_DISABLE(void)
{
    HAL_GPIO_WritePin(LSM303D_CS_PORT, LSM303D_CS_PIN, GPIO_PIN_SET);
}


uint8_t LSM303D_RW(uint8_t TxData)
{
    uint8_t RxData;
    HAL_SPI_TransmitReceive(&Spi1Handle, &TxData, &RxData, 1, 0x5000);
    return RxData;
}

void LSM303D_SET(uint8_t setAddr,uint8_t setData)
{
   LSM303D_CS_ENABLE();
   LSM303D_RW(setAddr);	//д��ַ
   LSM303D_RW(setData); // д����
   LSM303D_CS_DISABLE();
}






void EXTI4_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(LSM303D_ACC_DRDY_PIN) != RESET)
      {

       // LSM303D_ACC_READY(&LSM303D_ACC_report1);

        __HAL_GPIO_EXTI_CLEAR_IT(LSM303D_ACC_DRDY_PIN);
      }

}


void LSM303D_SPI_Read(uint8_t *Buffer, uint8_t ReadAddr,uint8_t NumByteToRead)
{
    uint8_t i;
    LSM303D_CS_ENABLE();
    LSM303D_RW(ReadAddr|=0xC0);  // �� ����ģʽ
    for(i=0;i<NumByteToRead;i++)
	{
        Buffer[i]=LSM303D_RW(0xFF);
    }
	LSM303D_CS_DISABLE();

}



void LSM303D_ACC_READY(LSM303D_ACC_report *pLSM303D_ACC_report)
{
    uint8_t LSM303D_ACC_buff[6]={0};

    LSM303D_SPI_Read( LSM303D_ACC_buff, LSM303D_OUT_X_L_A, 6);

    pLSM303D_ACC_report->timestamp = HAL_GetTick();
    pLSM303D_ACC_report->accel_x_raw =      (((int16_t)LSM303D_ACC_buff[1]) << 8) | LSM303D_ACC_buff[0];
    pLSM303D_ACC_report->accel_y_raw =      (((int16_t)LSM303D_ACC_buff[2]) << 8) | LSM303D_ACC_buff[2];
    pLSM303D_ACC_report->accel_z_raw =      (((int16_t)LSM303D_ACC_buff[3]) << 8) | LSM303D_ACC_buff[4];


    LSM303D_SPI_Read( LSM303D_ACC_buff, LSM303D_OUT_TEMP_L, 2);
    pLSM303D_ACC_report->temperature_raw =  (((int16_t)LSM303D_ACC_buff[1]) << 8) | LSM303D_ACC_buff[0];
    pLSM303D_ACC_report->flag = 1;
}



void EXTI1_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(LSM303D_MAG_DRDY_PIN) != RESET)
      {

        LSM303D_MAG_READY(&LSM303D_MAG_report1);
        LSM303D_RD_CNT++;
        __HAL_GPIO_EXTI_CLEAR_IT(LSM303D_MAG_DRDY_PIN);
      }

}

void LSM303D_MAG_READY(LSM303D_MAG_report *pLSM303D_MAG_report)
{
    uint8_t LSM303D_MAG_buff[6]={0};

    LSM303D_SPI_Read( LSM303D_MAG_buff, LSM303D_OUT_X_L_M, 6);

    pLSM303D_MAG_report->timestamp = HAL_GetTick();
    pLSM303D_MAG_report->mag_x_raw =      (((int16_t)LSM303D_MAG_buff[1]) << 8) | LSM303D_MAG_buff[0];
    pLSM303D_MAG_report->mag_y_raw =      (((int16_t)LSM303D_MAG_buff[2]) << 8) | LSM303D_MAG_buff[2];
    pLSM303D_MAG_report->mag_z_raw =      (((int16_t)LSM303D_MAG_buff[3]) << 8) | LSM303D_MAG_buff[4];

    LSM303D_SPI_Read( LSM303D_MAG_buff, LSM303D_OUT_TEMP_L, 2);
    pLSM303D_MAG_report->temperature_raw =  (((int16_t)LSM303D_MAG_buff[1]) << 8) | LSM303D_MAG_buff[0];
    pLSM303D_MAG_report->flag = 1;
}

