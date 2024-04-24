/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-25 下午7:05
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CYCLONE_PLATTER_APP_MICROS_H
#define CYCLONE_PLATTER_APP_MICROS_H
#include "main.h"

__STATIC_INLINE void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

__STATIC_INLINE void delay_us(uint32_t us)
{
    uint32_t us_count_tic =  us * (SystemCoreClock / 1000000U);
    DWT->CYCCNT = 0U;
    while(DWT->CYCCNT < us_count_tic);
}

__STATIC_INLINE uint32_t micros(void){
    return  DWT->CYCCNT / (SystemCoreClock / 1000000U);
}



#endif //CYCLONE_PLATTER_APP_MICROS_H
