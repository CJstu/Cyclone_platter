/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-25 上午11:37
* @version: 1.0
* @description: 
********************************************************************************/
#include <stdlib.h>
#include "Bsp_motor_pwm.h"
#include "tim.h"
#include "limits.h"



//uint32_t capture_value[3];    /*定义数组变量，存放捕获到的值*/
//uint32_t diff_value1;         /*计数差值*/
//uint32_t diff_value2;         /*计数差值*/
//
//uint8_t capture_state=0;     /*捕获状态：0表示未开始捕获；1表示完成一次捕获；2表示完成两次捕获*/
//uint8_t capture_flag=0;      /*捕获标志位：0表示未完成，1表示已完成*/


uint32_t Capture_Raise[IC_BufSize] = {0x00000000};  //CH1的DMA缓冲区，用于存放上升沿捕获数据
uint32_t Capture_Fall[IC_BufSize] = {0x00000000};   //CH2的DMA缓冲区，用于存放下降沿捕获数据

uint32_t Avg_Raise = 0x00000000;
uint32_t Avg_Fall = 0x00000000;

float Freq_Capture_Freq = 0.0;  //所测信号频率
float Freq_Capture_Duty = 0.0;  //所测信号占空比

float angle;

float angle_sd = 0.0;//初始化角度
float angle_sd_last = 0.0;//上一次的角度
uint32_t angle_sd_count = 0;//角度计数
float accumulated_angle = 0.0;


//初始化PWM
void pwm_Init(TIM_HandleTypeDef *htim,uint32_t Channel)
{
    HAL_TIM_PWM_Start( htim, Channel);
}

//修改pwm占空比
void pwm_SetDuty(TIM_HandleTypeDef *htim,uint32_t Channel,uint16_t duty)
{
    __HAL_TIM_SET_COMPARE(htim,Channel,duty);
}

void Freq_Meter_Init(void)
{
    //开始TIM2CH1的DMA传输
    HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_1, Capture_Raise, IC_BufSize);
    //还原TIM2状态，然后开始TIM2CH2的DMA传输
    htim3.State = HAL_TIM_STATE_READY;
    HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_2, Capture_Fall, IC_BufSize);
}

void Freq_Meter_DeInit(void)
{
    HAL_TIM_IC_Stop_DMA(&htim3, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_DMA(&htim3, TIM_CHANNEL_2);
}


void Freq_Calc(void)
{
    Avg_Raise = 0;
    Avg_Fall = 0;

    //取平均值
    for(uint16_t i = 0; i < IC_BufSize; i++)
    {
        Avg_Raise += Capture_Raise[i];
        Avg_Fall += Capture_Fall[i];
    }
    Avg_Raise /= IC_BufSize;
    Avg_Fall /= IC_BufSize;

    //计算频率及占空比
    if(Avg_Raise != 0)
    {
        Freq_Capture_Freq = (float)TIM3_Clock / (float)((TIM3_PSC + 1) * Avg_Raise);
        Freq_Capture_Duty = (float)Avg_Fall / (float)Avg_Raise;
    }
}

//float get_rewAngle(void)
//{
//        float rewAngle = get_angle(); //获取角度
//        float deltaAngle = angle_sd_last - rewAngle; //计算角度差
//        if (deltaAngle >= 300.0)
//        {
//            rewAngle = get_angle() + 360.0;
//            angle_sd_count ++;
//            return rewAngle;
//        }
//        else if (deltaAngle <= -300)
//        {
//            rewAngle = 0;
//            return rewAngle;
//        }
//        angle_sd_last = rewAngle;
//        return rewAngle;
//}

float get_angle(void)
{
    angle = 360.0/(4095.0/4119.0)*Freq_Capture_Duty;
    if (angle >= 360.0)
    {
        angle = 360.0;
    } else if (angle <= 0.0)
    {
        angle = 0.0;
    }
//    float angle[10];
//    for (uint8_t i = 0; i < 10; ++i)
//    {
//        HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
//        angle[i] = get_pwmar();
//    }
    return angle;
}

//float get_pwmar(void )
//{
//    if (capture_flag==1)    /* 捕获完成 */
//    {
////        Printf(USART1,"******capture start******\n");
//        if (capture_value[2] >= capture_value[0])    /* 计时没有溢出 */
//        {
//            diff_value1=capture_value[2]-capture_value[0];    /* 取差值 */
//        }
//        else    /* 计时有溢出 */
//        {
//            diff_value1=(0xffffffff+1)+capture_value[2]-capture_value[0];    /* 取差值 */
//        }
////        Printf(USART1,"周期：%.2fms\n",diff_value1/1000.0);    /*周期=差值*单次计数时间*/
////        Printf(USART1,"频率：%.2fKHz\n",1000.0/diff_value1);
//
//        if (capture_value[1] >= capture_value[0])    /* 计时没有溢出 */
//        {
//            diff_value2=capture_value[1]-capture_value[0];    /* 取差值 */
//        }
//        else    /* 计时有溢出 */
//        {
//            diff_value2=(0xffffffff+1)+capture_value[1]-capture_value[0];    /* 取差值 */
//        }
////        float angle = dutyCycleToAngle(diff_value2 / diff_value1);
//        angle = 360.0/(4095.0/4119.0)*diff_value2/diff_value1;
//        if (angle >= 360.0)
//        {
//            angle = 360.0;
//        } else if (angle <= 0.0)
//        {
//            angle = 0.0;
//        }
////        Printf(USART1,"占空比：%.2f%%\n",diff_value2*100.0/diff_value1);
////        Printf(USART1,"jiaodu:%.2f\n",angle);
////        Printf(USART1,"高电平：%.2fms\n",diff_value2/1000.0);    /*高电平时间=差值*单次计数时间*/
////        Printf(USART1,"占空比：%d%%\n",diff_value2*100/diff_value1);
////        Printf(USART1,"******capture over******\n");
////        printf("******capture over******\n");
//        capture_flag=0;    /*清除标志位，准备下一轮捕获*/
//        HAL_Delay(1);
//        HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
//        return angle;
//    }
//}
//

//
//float filterAndAverage(float * arr, int size)
//{
//    float min = INT_MAX;
//    float max = INT_MIN;
//    float sum = 0;
//    int count = 0;
//    // Find the min and max in the array
//    for(uint8_t i = 0; i < size; i++) {
//        if(arr[i] < min) {
//            min = arr[i];
//        }
//        if(arr[i] > max) {
//            max = arr[i];
//        }
//    }
//    // Sum the elements that are not equal to min or max
//    for(uint8_t i = 0; i < size; i++) {
//        if(arr[i] != min && arr[i] != max) {
//            sum += arr[i];
//            count++;
//        }
//    }
//    // Return the average
//    return (float)sum / count;
//}

/* USER CODE BEGIN 4 */
////回调函数
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim==&htim3)
//    {
//        switch (capture_state)
//        {
//            case 0:
//            {
//                capture_value[0]=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);    /*读取捕获开始时间*/
//                __HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);    /*切换为下降沿捕获*/
//                capture_state=1;    /*标志完成第一次捕获*/
//            }
//                break;
//
//            case 1:
//            {
//                capture_value[1]=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);    /*读取第二次捕获时间*/
//                __HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);    /*切换为上升沿捕获*/
//                capture_state=2;    /*标志完成第二次捕获*/
//            }
//                break;
//
//            case 2:
//            {
//                capture_value[2]=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);    /*读取捕获结束时间*/
//                HAL_TIM_IC_Stop_IT(htim,TIM_CHANNEL_1);    /*停止捕获*/
//                capture_state=0;    /*标志完成第三次捕获，重新置0，准备下一轮捕获*/
//                capture_flag=1;    /*标志捕获完成*/
//            }
//                break;
//
//            default:
////                printf("running error!\n");
//                break;
//        }
//    }
//
//}
/* USER CODE END 4 */
