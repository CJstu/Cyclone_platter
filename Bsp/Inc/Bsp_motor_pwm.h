/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-25 上午11:37
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CYCLONE_PLATTER_BSP_MOTOR_PWM_H
#define CYCLONE_PLATTER_BSP_MOTOR_PWM_H


#include "main.h"

//#define minDutyCycle 16.0 / 4119
//#define maxDutyCycle 4111.0 / 4119
//
//

//float get_pwmar(void );

//float filterAndAverage(float * arr, int size);
//void Freq_Meter_Init(void);
//void Freq_Meter_DeInit(void);

#define TIM3_Clock  84000000
#define TIM3_PSC    84 - 1
#define TIM3_ARR    65535

#define IC_BufSize  32          //DMA缓冲区大小

extern float Freq_Capture_Freq;
extern float Freq_Capture_Duty;
extern float angle;


void pwm_Init(TIM_HandleTypeDef *htim,uint32_t Channel);
void pwm_SetDuty(TIM_HandleTypeDef *htim,uint32_t Channel,uint16_t duty);
void Freq_Meter_Init(void);
void Freq_Meter_DeInit(void);
void Freq_Calc(void);
float get_angle(void);
float accumulate_angle(void );


#endif //CYCLONE_PLATTER_BSP_MOTOR_PWM_H
