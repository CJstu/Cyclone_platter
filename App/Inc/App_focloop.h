/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-25 上午11:23
* @version: 1.0
* @description: 
********************************************************************************/


#ifndef CYCLONE_PLATTER_APP_FOCLOOP_H
#define CYCLONE_PLATTER_APP_FOCLOOP_H

#include "main.h"
#include "arm_math.h"

#define _3PI_2 4.71238898038f

float constraint(float value,float min,float max);
float _electricalAngle(float shaft_angle, int pole_pairs);
float _normalizeAngle(float angle);
float _electricalAngle_wz();
void DFOC_alignSensor(int _PP,int _DIR);
void setPwm(float Ua, float Ub, float Uc);
void setPhaseVoltage(float Uq,float Ud, float angle_el);
void setTorque(float Uq,float angle_el);
float velocityOpenloop(float target_velocity);
void DFOC_Vbus(float power_supply);



#endif //CYCLONE_PLATTER_APP_FOCLOOP_H
