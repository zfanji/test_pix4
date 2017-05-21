#include "l3gd20h.h"
#include "main.h"

uint8_t initPass2=0;

void L3GD20_CS_ENABLE(void)
{
  HAL_GPIO_WritePin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_RESET);
 // 	HAL_GPIO_WritePin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_SET);
}

void L3GD20_CS_DISABLE(void)
{                
    HAL_GPIO_WritePin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(L3GD20_CS_PORT, L3GD20_CS_PIN, GPIO_PIN_RESET);
} 


#if 0
void L3GD20_Write(uint8_t* Write_Buffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  L3GD20_CS_LOW();
  
  /* Send the Address of the indexed register */
	SPI1_ReadWriteByte(SPI1,WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
		SPI1_ReadWriteByte(SPI1,*Write_Buffer);	
    NumByteToWrite--;
    Write_Buffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  L3GD20_CS_HIGH();
}


void L3GD20_Read(uint8_t* Read_Buffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  L3GD20_CS_LOW();
  
  /* Send the Address of the indexed register */
	SPI1_ReadWriteByte(SPI1,ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to L3GD20 (Slave device) */
    *Read_Buffer = SPI1_ReadWriteByte(SPI1,DUMMY_BYTE);		
    NumByteToRead--;
    Read_Buffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  L3GD20_CS_HIGH();
} 

#else
extern SPI_HandleTypeDef Spi1Handle;

uint8_t L3GD20_RW(uint8_t TxData)
{
    uint8_t RxData,ret;
    ret = HAL_SPI_TransmitReceive(&Spi1Handle, &TxData, &RxData, 1, 0x5000);
		if(ret==0){
			initPass2 = 1;
		}
    return RxData;
}

void L3GD20_Read(uint8_t* Read_Buffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
	if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  L3GD20_CS_ENABLE();
  
  /* Send the Address of the indexed register */
	L3GD20_RW(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to L3GD20 (Slave device) */
    *Read_Buffer = L3GD20_RW(DUMMY_BYTE);		
    NumByteToRead--;
    Read_Buffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  L3GD20_CS_DISABLE();
} 

void L3GD20_Write(uint8_t* Write_Buffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	/* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  L3GD20_CS_ENABLE();
  
  /* Send the Address of the indexed register */
	L3GD20_RW(WriteAddr);//写地址
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
		L3GD20_RW(*Write_Buffer);	// 写数据
    NumByteToWrite--;
    Write_Buffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  L3GD20_CS_DISABLE();

}


#endif


void L3GD20H_CS_Init(void)
{
#if 0
		//Define the corresponding structure
		GPIO_InitTypeDef GPIO_InitStructure;
		/* Enable INT1 GPIO clock */	
		RCC_AHB1PeriphClockCmd(L3GD20_SPI_CS_GPIO_CLK, ENABLE);

		/* Configure GPIO PIN for Lis Chip select */
		GPIO_InitStructure.GPIO_Pin = L3GD20_SPI_CS_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(L3GD20_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

		/* Deselect : Chip Select high */
		GPIO_SetBits(L3GD20_SPI_CS_GPIO_PORT, L3GD20_SPI_CS_PIN);
  
		/* Configure GPIO PINs to detect Interrupts */
		GPIO_InitStructure.GPIO_Pin = L3GD20_SPI_INT1_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_Init(L3GD20_SPI_INT1_GPIO_PORT, &GPIO_InitStructure); 
#endif
		    // IO初始化
    GPIO_InitTypeDef  GPIO_InitStruct;

    //L3GD20  片选IO
    L3GD20_CS_CLK_ENABLE();
    GPIO_InitStruct.Pin       = L3GD20_CS_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spi片选低有效，上拉保证稳定
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(L3GD20_CS_PORT, &GPIO_InitStruct);
    L3GD20_CS_DISABLE();

    //L3GD20 DRDY引脚 --外部中断 上升沿触发
    L3GD20_ACC_DRDY_CLK_ENABLE();
    GPIO_InitStruct.Pin       = L3GD20_ACC_DRDY_PIN;       // GPIO4 入口有讲究 需注意
    GPIO_InitStruct.Mode      = GPIO_MODE_IT_RISING; // 中断模式 上升沿中断
    GPIO_InitStruct.Pull      = GPIO_PULLUP;        // spi片选低有效，上拉保证稳定
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(L3GD20_ACC_DRDY_PORT, &GPIO_InitStruct);

    //外部中断初始化  acc_int
    HAL_NVIC_SetPriority(EXTI3_IRQn, 4, 0); // 设置外部引脚中断
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);         // 启动外部引脚中断
}  



uint8_t L3GD20_GetDataStatus(void)
{
  uint8_t L3GD20_Status; 
	
  /* Read STATUS_REG register */
   L3GD20_Read(&L3GD20_Status, L3GD20_STATUS_REG_ADDR, 1);   
	
  return L3GD20_Status;
}



void L3GD20_FilterCmd(uint8_t HighPassFilterState)
 {
  uint8_t L3GD20_HighPassFilterState;
	 
  /* Read CTRL_REG5 register */
   L3GD20_Read(&L3GD20_HighPassFilterState, L3GD20_CTRL_REG5_ADDR, 1);                  
  L3GD20_HighPassFilterState &= 0xEF;
  L3GD20_HighPassFilterState |= HighPassFilterState;
  /* Write value to MEMS CTRL_REG5 regsister */
   L3GD20_Write(&L3GD20_HighPassFilterState, L3GD20_CTRL_REG5_ADDR, 1);
}



void L3GD20_FilterConfig(L3GD20_FilterConfigTypeDef *L3GD20_FilterStruct) 
{
  uint8_t L3GD20_Modality;
  
  /* Read CTRL_REG2 register */
   L3GD20_Read(&L3GD20_Modality, L3GD20_CTRL_REG2_ADDR, 1);
	
  L3GD20_Modality &= 0xC0;
 
  /* Configure MEMS: mode and cutoff frquency */
  L3GD20_Modality |= (uint8_t) (L3GD20_FilterStruct->HighPassFilter_Mode_Selection |\
                      L3GD20_FilterStruct->HighPassFilter_CutOff_Frequency);                             

  /* Write value to MEMS CTRL_REG2 regsister */
   L3GD20_Write(&L3GD20_Modality, L3GD20_CTRL_REG2_ADDR, 1);
}



void L3GD20_INT1InterruptCmd(uint8_t InterruptState)
{  
  uint8_t L3GD20_INT1_InterruptCmd;
  
  /* Read CTRL_REG3 register */
   L3GD20_Read(&L3GD20_INT1_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
                  
  L3GD20_INT1_InterruptCmd &= 0x7F;	
  L3GD20_INT1_InterruptCmd |= InterruptState;
  
  /* Write value to MEMS CTRL_REG3 regsister */
  L3GD20_Write(&L3GD20_INT1_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_INT2InterruptCmd(uint8_t InterruptState)
{  
  uint8_t L3GD20_INT2_InterruptCmd;
  
  /* Read CTRL_REG3 register */
   L3GD20_Read(&L3GD20_INT2_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
                  
  L3GD20_INT2_InterruptCmd &= 0xF7;	
  L3GD20_INT2_InterruptCmd |= InterruptState;
  
  /* Write value to MEMS CTRL_REG3 regsister */
   L3GD20_Write(&L3GD20_INT2_InterruptCmd, L3GD20_CTRL_REG3_ADDR, 1);
}



void L3GD20_INT1InterruptConfig(L3GD20_InterruptConfigTypeDef *L3GD20_IntConfigStruct)
{
  uint8_t ctrl_cfr = 0x00, ctrl3 = 0x00;
  
  /* Read INT1_CFG register */
  L3GD20_Read(&ctrl_cfr, L3GD20_INT1_CFG_ADDR, 1);
  
  /* Read CTRL_REG3 register */
  L3GD20_Read(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
  
  ctrl_cfr &= 0x80;
  
  ctrl3 &= 0xDF;
  
  /* Configure latch Interrupt request and axe interrupts */                   
  ctrl_cfr |= (uint8_t)(L3GD20_IntConfigStruct->Latch_Request| \
                   L3GD20_IntConfigStruct->Interrupt_Axes);
                   
  ctrl3 |= (uint8_t)(L3GD20_IntConfigStruct->Interrupt_ActiveEdge);
  
  /* Write value to MEMS INT1_CFG register */
  L3GD20_Write(&ctrl_cfr, L3GD20_INT1_CFG_ADDR, 1);
  
  /* Write value to MEMS CTRL_REG3 register */
  L3GD20_Write(&ctrl3, L3GD20_CTRL_REG3_ADDR, 1);
}


void L3GD20_RebootCmd(void)
{
  uint8_t tmpreg;
  
  /* Read CTRL_REG5 register */
  L3GD20_Read(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
  
  /* Enable or Disable the reboot memory */
  tmpreg |= L3GD20_BOOT_REBOOTMEMORY;
  
  /* Write value to MEMS CTRL_REG5 regsister */
  L3GD20_Write(&tmpreg, L3GD20_CTRL_REG5_ADDR, 1);
}


void EXTI3_IRQHandler(void)
{
		DebugPrint("EXTI3_IRQHandler ~~~~~\r\n");
    if(__HAL_GPIO_EXTI_GET_IT(L3GD20_ACC_DRDY_PIN) != RESET)
      {
      	
        __HAL_GPIO_EXTI_CLEAR_IT(L3GD20_ACC_DRDY_PIN);
      }

}



void L3GD20_Init(void)
{  
  uint8_t ctrl1 = 0x00, ctrl4 = 0x00;
  
  /* Configure the low level interface ---------------------------------------*/
  L3GD20H_CS_Init();
 
#if 0	
  /* Configure MEMS: data rate, power mode, full scale and axes */
  ctrl1 |= (uint8_t) (L3GD20_InitStruct->Power_Mode | L3GD20_InitStruct->Output_DataRate | \
                    L3GD20_InitStruct->Axes_Enable | L3GD20_InitStruct->Band_Width);
  
  ctrl4 |= (uint8_t) (L3GD20_InitStruct->BlockData_Update | L3GD20_InitStruct->Endianness | \
                    L3GD20_InitStruct->Full_Scale);
#endif
	
  /* Write value to MEMS CTRL_REG1 regsister */
  L3GD20_Write(&ctrl1, L3GD20_CTRL_REG1_ADDR, 1);
  
  /* Write value to MEMS CTRL_REG4 regsister */
  L3GD20_Write(&ctrl4, L3GD20_CTRL_REG4_ADDR, 1);
	
	if(initPass2)
    	DebugPrint("L3G20H初始化完成.\r\n");
	else
		DebugPrint("L3G20H初始化失败!!!\r\n");
}







