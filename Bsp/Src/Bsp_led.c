/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-29 下午9:43
* @version: 1.0
* @description: 
********************************************************************************/
#include "Bsp_led.h"

//状态
void led_state(uint8_t ledState)
{
    if (ledState == 1)
    {
        HAL_GPIO_WritePin(TLED_GPIO_Port, TLED_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(TLED_GPIO_Port, TLED_Pin, GPIO_PIN_RESET);
    }
}

void led_fan(void)
{
    HAL_GPIO_TogglePin(TLED_GPIO_Port,TLED_Pin);
}

void led_buling(uint32_t time)
{
    HAL_GPIO_WritePin(TLED_GPIO_Port, TLED_Pin, GPIO_PIN_SET);
    HAL_Delay(time);
    HAL_GPIO_WritePin(TLED_GPIO_Port, TLED_Pin, GPIO_PIN_RESET);
    HAL_Delay(time);
}