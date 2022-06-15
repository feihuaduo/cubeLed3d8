#include "sys.h"
#include "usart.h"	
#include "config.h"
#include "string.h"
#include "stdio.h"
#include "queue.h"
////////////////////////////////////////////////////////////////////////////////// 	 
u8 usart_rx_buf[QUEUE_MSG_LEN];  //除去结束符最多接收(  QUEUE_MSG_LEN-2)个字符
s16 usart_rx_len = 0;         //接收长度标记	
u8 usart2_rx_buf[GATEWAY_QUEUE_MSG_LEN];     //接收缓冲,最大GATEWAY_QUEUE_MSG_LEN个字节
s16 usart2_rx_len = 0;         //接收长度标记

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//USART结构体
	NVIC_InitTypeDef NVIC_InitStructure;//中断结构体
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		usart_rx_buf[usart_rx_len]=USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		usart_rx_len++;
		if(usart_rx_len >= QUEUE_MSG_LEN)
			usart_rx_len=0;//接收数据错误或超出,重新开始接收  

		if(usart_rx_len > 0)
			if((usart_rx_buf[usart_rx_len-1] == '\r')||(usart_rx_buf[usart_rx_len-1] == '\n')){
				usart_rx_len--; //不保存\r\n
				usart_rx_buf[usart_rx_len] = 0;//针对字符串
				if((usart_rx_len > 0)&&(usart_rx_len < QUEUE_MSG_LEN))
					put_queue((char *)usart_rx_buf);//遇到\r或\n就保存一次数据到队列中
				usart_rx_len =0;//重新开始接收数据
			}				
    } 
}

#endif

//#if EN_USART2_RX   //如果使能了接收
////串口1中断服务程序
////注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
////接收状态
////bit15，	接收完成标志
////bit14，	接收到0x0d
////bit13~0，	接收到的有效字节数目
//u16 USART2_RX_STA=0;       //接收状态标记	

//int fputc2(u8   ch)
//{      
//	while((USART2->SR&0X40)==0)//循环发送,直到发送完毕   
//    USART2->DR = ch;      
//	return ch;
//}
//void dputs(char *str)
//{
//	while(*str)
//		fputc2(*str++);
//}
//void uart2_init(u32 bound)
//{
//   //GPIO端口设置
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
// 
//	//串口1对应引脚复用映射
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART3); //GPIOA10复用为USART1
//	
//	//USART1端口配置
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

//   //USART1 初始化设置
//	USART_InitStructure.USART_BaudRate = bound;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//  USART_Init(USART2, &USART_InitStructure); //初始化串口1
//	
//  USART_Cmd(USART2, ENABLE);  //使能串口1 
//	
//	//USART_ClearFlag(USART1, USART_FLAG_TC);
//	
//#if EN_USART2_RX	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

//	//Usart1 NVIC 配置
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

//#endif
//}
//#if 1//两种接收处理方式,第一种方式只能处理字符串,对来源不明的数据可以用方式二
////第一种方式为了不让新输入数据包含之前的信息,可以在新发字符串开头加上\r或\n,也就是\r\n起到了清空缓存的目的
//void USART2_IRQHandler(void)                	//串口3中断服务程序
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
//	{
//		usart_rx_buf[usart_rx_len]=USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
//		usart_rx_len++;
//		if(usart_rx_len >= QUEUE_MSG_LEN)
//			usart_rx_len=0;//接收数据错误或超出,重新开始接收  

//		if(usart_rx_len > 0)
//			if((usart_rx_buf[usart_rx_len-1] == '\r')||(usart_rx_buf[usart_rx_len-1] == '\n')){
//				usart_rx_len--; //不保存\r\n
//				usart_rx_buf[usart_rx_len] = 0;//针对字符串
//				if((usart_rx_len > 0)&&(usart_rx_len < QUEUE_MSG_LEN))
//					put_queue((char *)usart_rx_buf);//遇到\r或\n就保存一次数据到队列中
//				usart_rx_len =0;//重新开始接收数据
//			}				
//    }  
//}

s16 recv_gateway_data(void)
{
	char msgRec[GATEWAY_QUEUE_MSG_LEN];
	s16 total = 0;
	
	while(usart2_rx_len > 0){		
		if((total + usart2_rx_len)>=GATEWAY_QUEUE_MSG_LEN){					
			debug("error:long too len = %d\n",total + usart2_rx_len);
			break;
		}
		delay_ms(100);//必须要先延时才能保证数据收完
		memcpy(msgRec+total,usart2_rx_buf,usart2_rx_len);//循环读要放在select返回后，不能放在调select之前,也就是一次监听可能有多个未读完的数据
		total += usart2_rx_len;
		usart2_rx_len = 0;
		//debug("usart2_rx_len=%d,total=%d\n",usart2_rx_len,total);
		delay_ms(100);//等待一段时间后再次判断是否有数据要读取
	}
	msgRec[total] = '\0';//把收到的数据变成一个字符串
	gateway_put_queue(msgRec);
	return total;
}


#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
 
	

 



