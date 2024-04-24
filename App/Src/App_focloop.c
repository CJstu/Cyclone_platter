/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-25 上午11:23
* @version: 1.0
* @description: 
********************************************************************************/
#include "App_focloop.h"
#include "tim.h"

float voltage_limit=18;
float voltage_power_supply=0;
float shaft_angle=0,open_loop_timestamp=0;
float zero_electric_angle=0,Ualpha,Ubeta=0,Ua=0,Ub=0,Uc=0,dc_a=0,dc_b=0,dc_c=0;
int PP = 11;
int DIR = 1;

//约束函数
float constraint(float value,float min,float max)
{
    if(value>max)
        return max;
    else if(value<min)
        return min;
    else
        return value;
}

// 电角度求解（开环生成式）
float _electricalAngle(float shaft_angle, int pole_pairs)
{
    return (shaft_angle * pole_pairs);
}

// 电角度求解（位置闭环）
//编码器的值需要转化为弧度制
float _electricalAngle_wz()
{
    return  (_normalizeAngle((float)(DIR *  PP) * (get_angle()*PI/180)));
}

// 归一化角度到 [0,2PI]
float _normalizeAngle(float angle)
{
    float a = fmod(angle, 2*PI);
    return a >= 0 ? a : (a + 2*PI);
}

// 设置PWM到控制器输出
void setPwm(float Ua, float Ub, float Uc)
{
    // 计算占空比
    // 限制占空比从0到1
    dc_a = constraint(Ua / voltage_power_supply, 0.0f , 1.0f );
    dc_b = constraint(Ub / voltage_power_supply, 0.0f , 1.0f );
    dc_c = constraint(Uc / voltage_power_supply, 0.0f , 1.0f );

    //写入PWM
    pwm_SetDuty(&htim1,TIM_CHANNEL_1,dc_a*200);
    pwm_SetDuty(&htim1,TIM_CHANNEL_2,dc_b*200);
    pwm_SetDuty(&htim1,TIM_CHANNEL_3,dc_c*200);
}


// 设置相电压
void setPhaseVoltage(float Uq,float Ud, float angle_el)
{
    angle_el = _normalizeAngle(angle_el + zero_electric_angle);
    // 帕克逆变换
    Ualpha =  -Uq*arm_sin_f32(angle_el);
    Ubeta =   Uq*arm_cos_f32(angle_el);

    // 克拉克逆变换
    Ua = Ualpha + voltage_power_supply/2;
    Ub = (sqrt(3)*Ubeta-Ualpha)/2 + voltage_power_supply/2;
    Uc = (-Ualpha-sqrt(3)*Ubeta)/2 + voltage_power_supply/2;
    setPwm(Ua,Ub,Uc);
}


// 设置力矩
// angle_el  需要使用弧度制
void setTorque(float Uq,float angle_el)
{
    Uq=constraint(Uq,-voltage_power_supply/2,voltage_power_supply/2);
//    float Ud=0;
    angle_el = _normalizeAngle(angle_el);
    // 帕克逆变换
    Ualpha =  -Uq * arm_sin_f32(angle_el);
    Ubeta =   Uq * arm_cos_f32(angle_el);

    // 克拉克逆变换
    Ua = Ualpha + voltage_power_supply/2;
    Ub = (sqrt(3)*Ubeta-Ualpha)/2 + voltage_power_supply/2;
    Uc = (-Ualpha-sqrt(3)*Ubeta)/2 + voltage_power_supply/2;

    setPwm(Ua,Ub,Uc);
}

//初始化
void DFOC_Vbus(float power_supply)
{
    voltage_power_supply=power_supply;
    motor_Drive_Init();
    pwm_Init(&htim1,TIM_CHANNEL_1);
    pwm_Init(&htim1,TIM_CHANNEL_2);
    pwm_Init(&htim1,TIM_CHANNEL_3);
//    Serial.println("完成PWM初始化设置");
    Printf(USART1,"完成PWM初始化设置\r\n");
//    BeginSensor();//打开串口接收
}

//电机极对数 方向
void DFOC_alignSensor(int _PP,int _DIR)
{
    PP=_PP;
    DIR=_DIR;
    setTorque(4.5, _3PI_2);
    HAL_Delay(3000);
    zero_electric_angle=_electricalAngle_wz();
    setTorque(0, _3PI_2);
}

//开环速度函数
float velocityOpenloop(float target_velocity)
{
    unsigned long now_us = micros();  //获取从开启芯片以来的微秒数，它的精度是 4 微秒。 micros() 返回的是一个无符号长整型（unsigned long）的值
    //计算当前每个Loop的运行时间间隔
    float Ts = (now_us - open_loop_timestamp) * 1e-6f;
    //由于 micros() 函数返回的时间戳会在大约 70 分钟之后重新开始计数，在由70分钟跳变到0时，TS会出现异常，因此需要进行修正。如果时间间隔小于等于零或大于 0.5 秒，则将其设置为一个较小的默认值，即 1e-3f
    if(Ts <= 0 || Ts > 0.5f) Ts = 1e-3f;
    // 通过乘以时间间隔和目标速度来计算需要转动的机械角度，存储在 shaft_angle 变量中。在此之前，还需要对轴角度进行归一化，以确保其值在 0 到 2π 之间。
    shaft_angle = _normalizeAngle(shaft_angle + target_velocity*Ts);
    //以目标速度为 10 rad/s 为例，如果时间间隔是 1 秒，则在每个循环中需要增加 10 * 1 = 10 弧度的角度变化量，才能使电机转动到目标速度。
    //如果时间间隔是 0.1 秒，那么在每个循环中需要增加的角度变化量就是 10 * 0.1 = 1 弧度，才能实现相同的目标速度。因此，电机轴的转动角度取决于目标速度和时间间隔的乘积。
    // 使用早前设置的voltage_power_supply的1/3作为Uq值，这个值会直接影响输出力矩
    // 最大只能设置为Uq = voltage_power_supply/2，否则ua,ub,uc会超出供电电压限幅
    float Uq = voltage_power_supply/3;
    setPhaseVoltage(Uq,  0, _electricalAngle(shaft_angle, 11));
    open_loop_timestamp = now_us;  //用于计算下一个时间间隔
    return Uq;
}


