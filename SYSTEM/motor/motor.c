#include "motor.h" 



/*************************

*(A/B)IN1 -- PF(1/3)[+]

*(A/B)IN2 -- PF(2/4)[-]

************************/
  
//MOTOR IO初始化
void MOTOR_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOF,GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4);//GPIOF9,F10设置高，灯灭

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




