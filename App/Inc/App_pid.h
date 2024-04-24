/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-28 下午4:15
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CYCLONE_PLATTER_APP_PID_H
#define CYCLONE_PLATTER_APP_PID_H
#include "main.h"


//PID结构体
typedef struct
{
    float kp;                       //比例系数Proportional
    float ki;                       //积分系数Integral
    float kd;                       //微分系数Derivative
//	float ti;                       //积分时间常数
//  float td;                       //微分时间常数
//	float period;										//采样周期
    float ek;                       //当前误差
    float ek1;                      //前一次误差e(k-1)
    float ek2;                      //再前一次误差e(k-2)
    float location_sum;             //累计积分位置
    float out;											//PID输出值
}PID_LocTypeDef;



void PID_LocInit(PID_LocTypeDef *PID);
float PID_location(float setvalue, float actualvalue, PID_LocTypeDef *PID);


float PID_increment(float setvalue, float actualvalue, PID_LocTypeDef *PID);

#endif //CYCLONE_PLATTER_APP_PID_H
