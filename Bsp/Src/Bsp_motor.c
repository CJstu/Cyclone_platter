/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-25 下午1:07
* @version: 1.0
* @description: 
********************************************************************************/
#include "Bsp_motor.h"

void motor_Drive_Init(void)
{
    HAL_GPIO_WritePin(DRIVER_EN_GPIO_Port,DRIVER_EN_Pin,GPIO_PIN_SET);
}