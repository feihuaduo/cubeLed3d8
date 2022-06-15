#ifndef __INRARED_H
#define __INRARED_H	 
#include "sys.h" 


/*下面的方式是通过直接操作库函数方式读取IO*/
#define INRARED_L 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) //PA4
#define INRARED_M 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)	//PA3 
#define INRARED_R 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) //PA2
#define INRARED_W 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)	//PA0




#define INRARED_L_PRES 	1	//红外左按下
#define INRARED_M_PRES	2	//红外中按下
#define INRARED_R_PRES	3	//红外右按下
#define INRARED_W_PRES	4	//红外右按下

void INRARED_Init(void);	//IO初始化
//u8 INRARED_Scan(u8);  		//按键扫描函数	

#endif
