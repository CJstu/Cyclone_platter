/********************************************************************************
* @author: Zhao ChangJiang
* @email: hebuyijiangnan@gmail.com
* @date: 24-3-8 上午12:11
* @version: 1.0
* @description: 
********************************************************************************/

#include "Bsp_usart.h"


extern char CMD_Buffer[];////////////////////串口1
extern int CMD_Buffer_Count;
extern int CMD_Flag;

extern uint8_t usart_cmd_rx_buffer[];
/*********************************************************************************************************
*	函 数 名: Printf
        *	功能说明: 可变参数重定义Printf函数
        *	形    参: 1.串口号 2.char* fmt,...  格式化输出字符串和参数
        *	返 回 值: 无
*********************************************************************************************************
*/
#define    USART_SENDBUFFLEN     256          //定义打印的字符串数组长度

char g_USART_SendBuff[USART_SENDBUFFLEN];     //用于打印的字符串数组

void Printf(USART_TypeDef* USARTx, char* fmt, ...)
{

    uint16_t i = 0;                                //清空下标
    memset(g_USART_SendBuff,0,USART_SENDBUFFLEN); //清空字符数组

    va_list ap;                               //可变参数列表变量（对象）
    va_start(ap,fmt);                         //指明char*类型的fmt参数后面的才是可变参数的开始
    vsprintf(g_USART_SendBuff,fmt,ap);        //使用参数列表发送格式化输出到字符串
    va_end(ap);	                              //清空参数列表变量（对象）

    while(g_USART_SendBuff[i])                //如果字符串数组为NULL则退出打印
    {
        while((USARTx->SR&0X40)==0);
        USARTx->DR = g_USART_SendBuff[i++];
    }
}

/* USER CODE BEGIN 1 */
/**
  *brief  串口接收回调函数
  *note   消息格式: 消息头1 消息头2 消息长度 消息码 [消息内容] 校验码 消息尾1 消息尾2
  * 0x:0000000\n
  * 01:0000000\n   hi
  * 02:0000000\n   reboot
  * 03:0000000\n   led
  *param  *huart
  *retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    //换行符为0d0a
        if(usart_cmd_rx_buffer[0] == 0x0d)//\r
        {
            ;
        }
        else if(usart_cmd_rx_buffer[0] == 0x0a)//\n
        {
            CMD_Flag = 1;
            cmd();
        }
        else
        {
            CMD_Buffer[CMD_Buffer_Count] = usart_cmd_rx_buffer[0];
            CMD_Buffer_Count++;
        }
        HAL_UART_Receive_IT(&huart1, (uint8_t*)usart_cmd_rx_buffer, 1);
}
/* USER CODE END 1 */