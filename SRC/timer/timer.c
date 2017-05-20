


#include "stm32f4xx_hal.h"
#include "led.h"
#include "mpu6000.h"
#include "lsm303d.h"






TIM_HandleTypeDef    Tim5Handle;

uint16_t Timer5cnt = 0;
uint8_t debug_message = 0;



void TIM5_Init(void)
{


    // ��ʱ��
    __TIM5_CLK_ENABLE();

   Tim5Handle.Instance = TIM5;

   // ��ʱ1s
   //Tim5Handle.Init.Period = 10000 - 1;
   //  Tim5Handle.Init.Prescaler = (uint32_t) 16800/2-1;    // ��ʱ1S
   //Tim5Handle.Init.ClockDivision = 0;

   //��ʱ1ms
   Tim5Handle.Init.Period = 1000 - 1;
   Tim5Handle.Init.Prescaler = (uint32_t) 168-1;
   Tim5Handle.Init.ClockDivision = 0;

   Tim5Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
   HAL_TIM_Base_Init(&Tim5Handle);
   HAL_TIM_Base_Start_IT(&Tim5Handle);

   HAL_NVIC_SetPriority(TIM5_IRQn,5,0);
   HAL_NVIC_EnableIRQ(TIM5_IRQn);

}



void TIM5_IRQHandler(void)
{
    // �뼶����
    if(Timer5cnt < 1000){Timer5cnt++;}
    else {  Timer5cnt = 0;}

    //
    if( 0 == Timer5cnt)
    {
       debug_message = 1;
    }
    __HAL_TIM_CLEAR_IT(&Tim5Handle, TIM_IT_UPDATE);
}


