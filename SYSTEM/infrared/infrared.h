#ifndef __INRARED_H
#define __INRARED_H	 
#include "sys.h" 


/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define INRARED_L 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) //PA4
#define INRARED_M 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)	//PA3 
#define INRARED_R 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) //PA2
#define INRARED_W 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)	//PA0




#define INRARED_L_PRES 	1	//��������
#define INRARED_M_PRES	2	//�����а���
#define INRARED_R_PRES	3	//�����Ұ���
#define INRARED_W_PRES	4	//�����Ұ���

void INRARED_Init(void);	//IO��ʼ��
//u8 INRARED_Scan(u8);  		//����ɨ�躯��	

#endif
