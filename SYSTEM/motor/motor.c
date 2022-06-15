#include "motor.h" 



/*************************

*(A/B)IN1 -- PF(1/3)[+]

*(A/B)IN2 -- PF(2/4)[-]

************************/
  
//MOTOR IO��ʼ��
void MOTOR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOF,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4);//GPIOF9,F10���øߣ�����

}

void moto_ops(s16 left,s16 right)
{
	if(left<0){
		GPIO_SetBits(GPIOF, GPIO_Pin_1 );				    
		GPIO_ResetBits(GPIOF, GPIO_Pin_2 );
	}else if(left>0){
		GPIO_SetBits(GPIOF, GPIO_Pin_2 );
		GPIO_ResetBits(GPIOF, GPIO_Pin_1 ); 
	}else{
		GPIO_SetBits(GPIOF, GPIO_Pin_1 );
		GPIO_SetBits(GPIOF, GPIO_Pin_2 ); 	
	}
	if(right<0){
		GPIO_SetBits(GPIOF, GPIO_Pin_4 );				    
		GPIO_ResetBits(GPIOF, GPIO_Pin_3 );
	}else if(right>0){
		GPIO_SetBits(GPIOF, GPIO_Pin_3 );
		GPIO_ResetBits(GPIOF, GPIO_Pin_4 ); 
	}else{
		GPIO_SetBits(GPIOF, GPIO_Pin_3 );
		GPIO_SetBits(GPIOF, GPIO_Pin_4 ); 	
	}
}




