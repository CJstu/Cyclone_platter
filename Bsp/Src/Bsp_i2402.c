/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-30 下午9:50
* @version: 1.0
* @description: 
********************************************************************************/
#include "Bsp_i2402.h"
#include "adc.h"


volatile uint32_t AdcBuf[3] = {0};

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
//        AdcBuf[0] = HAL_ADC_GetValue(&hadc1);
        Printf(USART1,"ADC1:%d  ADC2:%d  ADC3:%d\r\n",AdcBuf[0],AdcBuf[1],AdcBuf[2]);
    }
}



