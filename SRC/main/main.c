

#include "main.h"
#include "timer.h"
#include "spi.h"
#include "mpu6000.h"
#include "lsm303d.h"
#include "i2c.h"
#include "rgbled.h"
#include "l3gd20h.h"
#include "ms5611.h"

extern uint16_t     debug_message;
extern MPU_report   MPU_report1;
extern uint16_t     MPU_RD_CNT;

extern LSM303D_ACC_report  LSM303D_ACC_report1;
extern LSM303D_MAG_report  LSM303D_MAG_report1;
extern uint16_t     LSM303D_RD_CNT;

extern _MS5611_NORMAL_DATA_SHARED MS5611_NORMAL_DATA_SHARED;

void assert_failed(uint8_t* file, uint32_t line)
{
    while(1)
    {
        DebugPrint("配置检查错误!!!!");
        HAL_Delay(250);
    }

}

// 开启系统时钟配置
/// System Clock Configuration
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    //启动电源管理时钟
  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

    // 调整电源工作模式  设置为默认模式 11
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;    // 外部晶振
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;  // 晶振开
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; // PLL 开
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  // PLL外部晶振输入
  RCC_OscInitStruct.PLL.PLLM = 24;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


uint8_t chLed = 0;
uint8_t l3gd20Sta = 0;

int main(void)
{

    HAL_Init();         // 初始化节拍器
    SystemClock_Config();   // 初始化时钟
    LED_Init(LED_AMBER);        // 初始化LED
    UART7_Init();

    POWER_Init();
    //1秒一次定时器  用于刷主循环
    TIM5_Init();

	I2C2_Init();
    RGBLED_Init();

	MPU6000_CS_init();
	LSM303D_CS_init();
	L3GD20_CS_init();
	MS5611_CS_init();
	
    SPI1_Init();     // 初始化SPI1 用于操作传感器

    MPU6000_Init();  // 初始化MPU6000
    LSM303D_Init();
  	L3GD20_Init();
	MS5611_Init();

	DebugPrint("\r\n \r\n StartLoop...... \r\n");
	
    while (1)
    {
        if(1 == debug_message)
        {
            debug_message = 0;
            LED_Toggle(LED_AMBER);

#if 1/////////////////////////TCA62724
			if(chLed==0){
				setRGBLED(1,0,0);	
				chLed = 1;
			}else if(chLed==1){
				chLed = 2;
				setRGBLED(0,1,0);
			}else if(chLed==2){
				chLed = 0;
				setRGBLED(0,0,1);
			}
#endif		
						
#if 1/////////////////////////= L3GD20
			l3gd20Sta = L3GD20_GetDataStatus();
			DebugPrint("l3gd20Sta=0x%x \r\n",l3gd20Sta);
#endif		
				
#if 1/////////////////////////mpu6000
			is_mpu6000();
            DebugPrint("MPU6000 ACCEL x=%d, y=%d, z=%d\r\n",
                    MPU_report1.accel_x_raw,
                    MPU_report1.accel_y_raw,
                    MPU_report1.accel_z_raw);

            MPU_RD_CNT=0;
			DebugPrint("mpu600 cnt=%d\r\n\r\n",MPU_RD_CNT);
#endif
					
#if 1/////////////////////////LSM303D
			who_am_i();
            DebugPrint("LSM303D ACCEL x=%d, y=%d, z=%d\r\n",
                          LSM303D_ACC_report1.accel_x_raw,
                          LSM303D_ACC_report1.accel_y_raw,
                          LSM303D_ACC_report1.accel_z_raw);
            DebugPrint("lsm303d cnt=%d\r\n\r\n",LSM303D_RD_CNT);
            LSM303D_RD_CNT=0;
#endif		


#if 1/////////////////////////Ms5611
			getMs5611Data();
#endif
        }

    }

}





