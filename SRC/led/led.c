#include "led.h"


GPIO_TypeDef* GPIO_PORT[LEDn] ={ LED_AMBER_GPIO_PORT };
const uint16_t GPIO_PIN[LEDn] ={ LED_AMBER_PIN };


void LED_Init(Led_TypeDef Led)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // ��ʱ��
    LED_AMBER_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN[Led];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIO_PORT[Led],&GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIO_PORT[Led],GPIO_PIN[Led],GPIO_PIN_RESET);


}


void LED_Off(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET);
}

void LED_On(Led_TypeDef Led)
{
    HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
}


void LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}




//VDD_BRICK_VALID
//VDD_SERVO_VALID
//VBUS_BALID   ������ų�ʼ��
static void VDD_VALID_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // ��ʱ��
    GPIO_VDD_BRICK_VALID_CLK_ENABLE();
    GPIO_VDD_SERVO_VALID_CLK_ENABLE();
    GPIO_VBUS_VALID_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_VDD_BRICK_VALID_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIO_VDD_BRICK_VALID_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_VDD_SERVO_VALID_PIN;
    HAL_GPIO_Init(GPIO_VDD_SERVO_VALID_PORT,&GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_VBUS_VALID_PIN;
    HAL_GPIO_Init(GPIO_VDD_SERVO_VALID_PORT,&GPIO_InitStruct);

}

void Display_Power(void)
{
    GPIO_PinState       VDD_BRICK_VALID;
    GPIO_PinState       VDD_SERVO_VALID;
    GPIO_PinState       VBUS_VALID;

    VDD_BRICK_VALID = HAL_GPIO_ReadPin(GPIO_VDD_BRICK_VALID_PORT,GPIO_VDD_BRICK_VALID_PIN);
    VDD_SERVO_VALID = HAL_GPIO_ReadPin(GPIO_VDD_SERVO_VALID_PORT,GPIO_VDD_SERVO_VALID_PIN);
    VBUS_VALID = HAL_GPIO_ReadPin(GPIO_VBUS_VALID_PORT,GPIO_VBUS_VALID_PIN);

    if( VDD_BRICK_VALID == GPIO_PIN_RESET)
    {
        DebugPrint("VDD_BRICK_VALID -- ��Դ�ɵ���ṩ\r\n");
    }


    if( VDD_SERVO_VALID == GPIO_PIN_RESET)
    {
        DebugPrint("VDD_SERVO_VALID -- ��Դ�ɵ���ṩ\r\n");
    }

    if( VBUS_VALID == GPIO_PIN_RESET)
    {
        DebugPrint("VDD_BRICK_VALID -- ��Դ��USB�ṩ\r\n");
    }

}



static void VDD_CTRL_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // ��ʱ��
    GPIO_VDD_5V_PERIPH_EN_CLK_DISABLE();    // ����Ч
    GPIO_VDD_3V3_SENSORS_EN_CLK_ENABLE();   // ����Ч
    GPIO_VDD_5V_HIPOWER_OC_CLK_ENABLE();    // OC  ����Ч
    GPIO_VDD_5V_PERIPH_OC_CLK_ENABLE();     //  OC  ����Ч


    GPIO_InitStruct.Pin = GPIO_VDD_5V_PERIPH_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIO_VDD_5V_PERIPH_EN_PORT,&GPIO_InitStruct);
    VDD_5V_PERIPH_EN_ENABLE();

    GPIO_InitStruct.Pin = GPIO_VDD_3V3_SENSORS_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIO_VDD_3V3_SENSORS_EN_PORT,&GPIO_InitStruct);
    VDD_3V3_SENSORS_EN_ENABLE();



    GPIO_InitStruct.Pin = GPIO_VDD_5V_HIPOWER_OC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIO_VDD_5V_HIPOWER_OC_PORT,&GPIO_InitStruct);
    VDD_5V_HIPOWER_OC_ENABLE();

    GPIO_InitStruct.Pin = GPIO_VDD_5V_PERIPH_OC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIO_VDD_5V_PERIPH_OC_PORT,&GPIO_InitStruct);
    VDD_5V_PERIPH_OC_ENABLE();

}


// ����Ч
void VDD_5V_PERIPH_EN_ENABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_5V_PERIPH_EN_PORT, GPIO_VDD_5V_PERIPH_EN_PIN,GPIO_PIN_RESET);
    DebugPrint("VDD_5V_PERIPH_EN -- ��ʹ��\r\n");
}

void VDD_5V_PERIPH_EN_DISABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_5V_PERIPH_EN_PORT, GPIO_VDD_5V_PERIPH_EN_PIN,GPIO_PIN_SET);
    DebugPrint("VDD_5V_PERIPH_EN -- �ѹر�\r\n");
}

// ��ʹ��
void VDD_3V3_SENSORS_EN_ENABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_3V3_SENSORS_EN_PORT, GPIO_VDD_3V3_SENSORS_EN_PIN,GPIO_PIN_SET);
    DebugPrint("VDD_3V3_SENSORS_EN -- ��ʹ��\r\n");
}

void VDD_3V3_SENSORS_EN_DISABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_3V3_SENSORS_EN_PORT, GPIO_VDD_3V3_SENSORS_EN_PIN,GPIO_PIN_RESET);
    DebugPrint("VDD_3V3_SENSORS_EN -- �ѹر�\r\n");
}


// ��ʹ��
void VDD_5V_HIPOWER_OC_ENABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_5V_HIPOWER_OC_PORT, GPIO_VDD_5V_HIPOWER_OC_PIN,GPIO_PIN_SET);
    DebugPrint("VDD_5V_HIPOWER_OC -- ��ʹ��\r\n");
}

void VDD_5V_HIPOWER_OC_DISABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_5V_PERIPH_OC_PORT, GPIO_VDD_5V_PERIPH_OC_PIN,GPIO_PIN_RESET);
    DebugPrint("VDD_5V_HIPOWER_OC -- �ѹر�\r\n");
}


void VDD_5V_PERIPH_OC_ENABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_5V_PERIPH_OC_PORT, GPIO_VDD_5V_PERIPH_OC_PIN,GPIO_PIN_SET);
    DebugPrint("VDD_5V_HIPOWER_OC -- ��ʹ��\r\n");
}

void VDD_5V_PERIPH_OC_DISABLE(void)
{
    HAL_GPIO_WritePin(GPIO_VDD_5V_PERIPH_OC_PORT, GPIO_VDD_5V_PERIPH_OC_PIN,GPIO_PIN_RESET);
    DebugPrint("VDD_5V_HIPOWER_OC -- �ѹر�\r\n");
}


void POWER_Init(void)
{
    //  ��·��Դ���
    VDD_VALID_Init();
    Display_Power();

    //�������ܿ��Ƶĸ�·��Դ
    VDD_CTRL_Init();
}

