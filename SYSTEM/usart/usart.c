#include "sys.h"
#include "usart.h"	
#include "config.h"
#include "string.h"
#include "stdio.h"
#include "queue.h"
////////////////////////////////////////////////////////////////////////////////// 	 
u8 usart_rx_buf[QUEUE_MSG_LEN];  //��ȥ������������(  QUEUE_MSG_LEN-2)���ַ�
s16 usart_rx_len = 0;         //���ճ��ȱ��	
u8 usart2_rx_buf[GATEWAY_QUEUE_MSG_LEN];     //���ջ���,���GATEWAY_QUEUE_MSG_LEN���ֽ�
s16 usart2_rx_len = 0;         //���ճ��ȱ��

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;//USART�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		usart_rx_buf[usart_rx_len]=USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		usart_rx_len++;
		if(usart_rx_len >= QUEUE_MSG_LEN)
			usart_rx_len=0;//�������ݴ���򳬳�,���¿�ʼ����  

		if(usart_rx_len > 0)
			if((usart_rx_buf[usart_rx_len-1] == '\r')||(usart_rx_buf[usart_rx_len-1] == '\n')){
				usart_rx_len--; //������\r\n
				usart_rx_buf[usart_rx_len] = 0;//����ַ���
				if((usart_rx_len > 0)&&(usart_rx_len < QUEUE_MSG_LEN))
					put_queue((char *)usart_rx_buf);//����\r��\n�ͱ���һ�����ݵ�������
				usart_rx_len =0;//���¿�ʼ��������
			}				
    } 
}

#endif

//#if EN_USART2_RX   //���ʹ���˽���
////����1�жϷ������
////ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
////����״̬
////bit15��	������ɱ�־
////bit14��	���յ�0x0d
////bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART2_RX_STA=0;       //����״̬���	

//int fputc2(u8   ch)
//{      
//	while((USART2->SR&0X40)==0)//ѭ������,ֱ���������   
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
//   //GPIO�˿�����
//  GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
// 
//	//����1��Ӧ���Ÿ���ӳ��
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9����ΪUSART1
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART3); //GPIOA10����ΪUSART1
//	
//	//USART1�˿�����
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
//	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

//   //USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = bound;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
//	
//  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���1 
//	
//	//USART_ClearFlag(USART1, USART_FLAG_TC);
//	
//#if EN_USART2_RX	
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

//#endif
//}
//#if 1//���ֽ��մ���ʽ,��һ�ַ�ʽֻ�ܴ����ַ���,����Դ���������ݿ����÷�ʽ��
////��һ�ַ�ʽΪ�˲������������ݰ���֮ǰ����Ϣ,�������·��ַ�����ͷ����\r��\n,Ҳ����\r\n������ջ����Ŀ��
//void USART2_IRQHandler(void)                	//����3�жϷ������
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
//	{
//		usart_rx_buf[usart_rx_len]=USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
//		usart_rx_len++;
//		if(usart_rx_len >= QUEUE_MSG_LEN)
//			usart_rx_len=0;//�������ݴ���򳬳�,���¿�ʼ����  

//		if(usart_rx_len > 0)
//			if((usart_rx_buf[usart_rx_len-1] == '\r')||(usart_rx_buf[usart_rx_len-1] == '\n')){
//				usart_rx_len--; //������\r\n
//				usart_rx_buf[usart_rx_len] = 0;//����ַ���
//				if((usart_rx_len > 0)&&(usart_rx_len < QUEUE_MSG_LEN))
//					put_queue((char *)usart_rx_buf);//����\r��\n�ͱ���һ�����ݵ�������
//				usart_rx_len =0;//���¿�ʼ��������
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
		delay_ms(100);//����Ҫ����ʱ���ܱ�֤��������
		memcpy(msgRec+total,usart2_rx_buf,usart2_rx_len);//ѭ����Ҫ����select���غ󣬲��ܷ��ڵ�select֮ǰ,Ҳ����һ�μ��������ж��δ���������
		total += usart2_rx_len;
		usart2_rx_len = 0;
		//debug("usart2_rx_len=%d,total=%d\n",usart2_rx_len,total);
		delay_ms(100);//�ȴ�һ��ʱ����ٴ��ж��Ƿ�������Ҫ��ȡ
	}
	msgRec[total] = '\0';//���յ������ݱ��һ���ַ���
	gateway_put_queue(msgRec);
	return total;
}


#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
 
	

 



