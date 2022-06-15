#include "stm32f4xx.h"

#include "word.h"
#include "heart.h"
#include "circle.h"
#include "circle_a.h"
#include "gogo.h"

#define u8 uint8_t
#define u16 uint16_t

#define DATA_GPIO GPIOA
#define LATCH_GPIO GPIOC
#define LAYER_GPIO GPIOD

GPIO_InitTypeDef  GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

u8 dataPin[]={1,2,3,4,5,6,7,8};
u8 latchPin[]={0,1,2,3,4,5,6,7};
u8 layerPin[]={0,1,2,3,4,5,6,7};

//设置引脚电平
void setPin(GPIO_TypeDef* GPIOx,u8 num,int bits);
//设置层
void setLayer(u8 data);
//设置哪一版
void setLatch(u8 data);
//设置数据
void setData(u8 data);

//设置层数，0~7
void setLayerNum(u8 num);
//设置哪一版，0~7
void setLatchNum(u8 num);

void delay_ms(uint32_t nms);
void delay_us(uint32_t ums);

void delay_init(void);

void display(char dat[][8],int go_speed);
void init(void);

void gogo(void);
void heart(void);
void circle(void);
void circle_a(void);
void word(void);

void demo1();
void demo2();

int main()
{
	init();

	delay_ms(500);
	demo1();
	
}

void demo1(){
	int i,j;
	u8 latch=0x01;
	u8 data;
	
	while(1){
		//每层
		setLayer(0xff);
		
		latch=0x01;
		for(i=0;i<8;i++){
			setLatch(latch);
			data=0x01;
			for(j=0;j<8;j++){
				setData(data);
				delay_ms(12);
				data<<=1;
			}
			latch<<=1;
		}
		
		latch=0x80;
		for(i=0;i<8;i++){
			setLatch(latch);
			data=0x80;
			for(j=0;j<8;j++){
				setData(data);
				delay_ms(12);
				data>>=1;
			}
			latch>>=1;
		}
		
		gogo();
		setLatch(0xff);
		setLayer(0xff);
		setData(0x00);
		delay_ms(1000);
		
	}
}

void demo2(){
	while(1){
		     
        heart();
        heart();
        heart();

        circle();
        delay_ms(1000);
        circle_a();
        delay_ms(1000);
        word();
        
        gogo();
	}
}

void init(){
		/*????GPIO_InitTypeDef??????*/
		GPIO_InitTypeDef GPIO_InitStructure;
		/*??GPIOB?????*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
		GPIO_Init(GPIOA, &GPIO_InitStructure);//???

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
		GPIO_Init(GPIOC, &GPIO_InitStructure);//???

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//??????
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//??
		GPIO_Init(GPIOD, &GPIO_InitStructure);//???
		
		delay_init();
		
		setLatch(0xff);
		setLayer(0xff);
		setData(0x00);
}

void display(char dat[][8],int go_speed)
{
    char speed;
    char x_line;
    char z_line;
    speed=go_speed*7;
    while(speed--)
    {
        for(x_line=0; x_line<8; x_line++)
        {
            setLatchNum(x_line);

            for(z_line=0; z_line<8; z_line++)
            {
                setLayerNum(z_line);
               setData(dat[x_line][z_line]);
            }
        }
    }

}

void setLayerNum(u8 num){
	if(num>7)
		return;
	setLayer(0x01<<num);
}

void setLatchNum(u8 num){
	if(num>7)
		return;
	setLatch(0x01<<num);
}

void setLayer(u8 data){
	int i;
	for(i=0;i<8;i++){
		setPin(LAYER_GPIO,layerPin[i],data&(0x01<<i));
	}
}

void setLatch(u8 data){
	int i;
	for(i=0;i<8;i++){
		setPin(LATCH_GPIO,latchPin[7-i],data&(0x01<<i));
	}
}

void setData(u8 data){
	int i;
	for(i=0;i<8;i++){
		setPin(DATA_GPIO,dataPin[i],data&(0x01<<i));
	}
}

void setPin(GPIO_TypeDef* GPIOx,u8 num,int bits){
	uint16_t pin;
	switch(num){
		case 0:
			pin=GPIO_Pin_0;
			break;
		case 1:
			pin=GPIO_Pin_1;
			break;
		case 2:
			pin=GPIO_Pin_2;
			break;
		case 3:
			pin=GPIO_Pin_3;
			break;
		case 4:
			pin=GPIO_Pin_4;
			break;
		case 5:
			pin=GPIO_Pin_5;
			break;
		case 6:
			pin=GPIO_Pin_6;
			break;
		case 7:
			pin=GPIO_Pin_7;
			break;
		case 8:
			pin=GPIO_Pin_8;
			break;
		case 9:
			pin=GPIO_Pin_9;
			break;
		case 10:
			pin=GPIO_Pin_10;
			break;
		case 11:
			pin=GPIO_Pin_11;
			break;
		case 12:
			pin=GPIO_Pin_12;
			break;
		case 13:
			pin=GPIO_Pin_13;
			break;
		case 14:
			pin=GPIO_Pin_14;
			break;
		case 15:
			pin=GPIO_Pin_15;
			break;
	}
	if(bits)
		GPIO_SetBits(GPIOx, pin);
	else
		GPIO_ResetBits(GPIOx, pin);	
}


void delay_ms(uint32_t nms){
	uint32_t m,n;
	
	m = nms/500;
	
	n = nms%500;
	
	while(m--)
	{
		SysTick->LOAD  = (SystemCoreClock /8/1000) * 500;	//??n??
		SysTick->CTRL |= 1;									//?????????
		while((SysTick->CTRL & (1<<16) )== 0);				//??????????
		SysTick->CTRL &=~1;									//?????????		
	
	}

	if(n)
	{
		SysTick->LOAD  = (SystemCoreClock /8/1000) * n;			//??n??

		SysTick->CTRL |= 1;										//?????????
		while((SysTick->CTRL & (1<<16) )== 0);					//??????????
		SysTick->CTRL &=~1;										//?????????	
		
	}

}


void delay_init() 
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseStructure.TIM_Period = 83;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
}

//??nus
//nus?????us?.                                               
void delay_us(u32 nus)
{
    u32 cnt;
    cnt = nus;
    TIM_Cmd(TIM4, ENABLE);
    while(cnt--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE);
}


void gogo()
{
    display(gogo1,5);
    display(gogo2,5);
    display(gogo3,5);
    display(gogo4,5);
    display(gogo5,5);
    display(gogo6,5);
    display(gogo7,5);
    display(gogo8,5);
	
    display(gogo9,5);
    display(gogo10,5);
    display(gogo11,5);
    display(gogo12,5);
    display(gogo13,5);
    display(gogo14,5);
    display(gogo15,5);
	
    display(gogo14,5);
    display(gogo13,5);
    display(gogo12,5);
    display(gogo11,5);
    display(gogo10,5);
    display(gogo9,5);
    display(gogo8,5);
		
    display(gogo9,5);
    display(gogo10,5);
    display(gogo11,5);
    display(gogo12,5);
    display(gogo13,5);
    display(gogo14,5);
    display(gogo15,5);
		
    display(gogo14,5);
    display(gogo13,5);
    display(gogo12,5);
    display(gogo11,5);
    display(gogo10,5);
    display(gogo9,5);
    display(gogo8,5);
		
    display(gogo23,5);
    display(gogo24,5);
    display(gogo25,5);
    display(gogo26,5);
    display(gogo27,5);
    display(gogo28,5);
    display(gogo29,5);
    display(gogo30,5);
    display(gogo31,5);
    display(gogo32,5);
    display(gogo33,5);
    display(gogo34,5);
    display(gogo35,5);
    display(gogo36,5);
    display(gogo37,5);
    display(gogo38,5);
    display(gogo39,5);
    display(gogo40,5);
    display(gogo41,5);
    display(gogo42,5);
    display(gogo43,5);
    display(gogo42,5);
    display(gogo41,5);
    display(gogo40,5);
    display(gogo39,5);
    display(gogo38,5);
    display(gogo37,5);
    display(gogo36,5);
    display(gogo37,5);
    display(gogo38,5);
    display(gogo39,5);
    display(gogo40,5);
    display(gogo41,5);
    display(gogo42,5);
    display(gogo43,5);
    display(gogo42,5);
    display(gogo41,5);
    display(gogo40,5);
    display(gogo39,5);
    display(gogo38,5);
    display(gogo37,5);
    display(gogo36,5);
    display(gogo51,5);
    display(gogo52,5);
    display(gogo53,5);
    display(gogo54,5);
    display(gogo55,5);
    display(gogo56,5);
    display(gogo57,5);
    display(gogo58,5);
    display(gogo59,5);
    display(gogo60,5);
    display(gogo61,5);
    display(gogo62,5);
    display(gogo63,5);
    display(gogo64,5);
    display(gogo65,5);
    display(gogo66,5);
    display(gogo67,5);
    display(gogo68,5);
    display(gogo69,5);
    display(gogo70,5);
    display(gogo71,5);
    display(gogo72,5);
    display(gogo73,5);
    display(gogo74,5);
    display(gogo75,5);
    display(gogo76,5);
    display(gogo77,5);
    display(gogo76,5);
    display(gogo75,5);
    display(gogo74,5);
    display(gogo73,5);
    display(gogo72,5);
    display(gogo71,5);
    display(gogo70,5);
    display(gogo69,5);
    display(gogo68,5);
    display(gogo67,5);
    display(gogo66,5);
    display(gogo65,5);
    display(gogo64,5);
    display(gogo65,5);
    display(gogo66,5);
    display(gogo67,5);
    display(gogo68,5);
    display(gogo69,5);
    display(gogo70,5);
    display(gogo71,5);
    display(gogo72,5);
    display(gogo73,5);
    display(gogo74,5);
    display(gogo75,5);
    display(gogo76,5);
    display(gogo77,5);
    display(gogo76,5);
    display(gogo75,5);
    display(gogo74,5);
    display(gogo73,5);
    display(gogo72,5);
    display(gogo71,5);
    display(gogo70,5);
    display(gogo69,5);
    display(gogo68,5);
    display(gogo67,5);
    display(gogo66,5);
    display(gogo65,5);
    display(gogo64,5);
    display(gogo91,5);
    display(gogo92,5);
    display(gogo93,5);
    display(gogo94,5);
    display(gogo95,5);
    display(gogo96,5);
    display(gogo97,5);
    display(gogo98,5);
    display(gogo99,5);
    display(gogo101,5);
    display(gogo102,5);
    display(gogo103,5);
    display(gogo104,5);
    display(gogo103,5);
    display(gogo102,5);
    display(gogo101,5);
    display(gogo100,5);
    display(gogo99,5);
    display(gogo98,5);
    display(gogo97,5);
    display(gogo98,5);
    display(gogo99,5);
    display(gogo101,5);
    display(gogo102,5);
    display(gogo103,5);
    display(gogo104,5);
    display(gogo105,5);
    display(gogo106,5);
    display(gogo107,5);
    display(gogo108,5);
    display(gogo109,5);
    display(gogo110,5);
    display(gogo111,5);
    display(gogo100,5);
    
}

void heart()
{
    display(heart1,5);
    display(heart2,5);
    display(heart3,5);
    display(heart4,5);
    display(heart5,5);
    display(heart6,5);
    display(heart7,5);
    display(heart8,5);
    display(heart9,5);
    display(heart10,5);
    display(heart11,5);
    display(heart12,5);
    display(heart13,5);
    display(heart14,5);
    display(heart15,5);
}

void circle()
{
    //----------1--------//
    display(circle142,5);
    display(circle141,5);
    display(circle140,5);
    display(circle139,5);
    display(circle138,5);
    display(circle137,5);
    display(circle136,5);
    display(circle135,5);
    display(circle134,5);
    display(circle133,5);
    display(circle132,5);
    display(circle131,5);
    display(circle130,5);
    display(circle129,5);
    display(circle128,5);
    display(circle127,5);
    display(circle126,5);
    display(circle125,5);
    display(circle124,5);
    display(circle123,5);
    display(circle122,5);
    display(circle121,5);
    display(circle120,5);
    display(circle119,5);
    display(circle118,5);
    display(circle117,5);
    display(circle116,5);
    display(circle115,5);
    display(circle114,5);
    display(circle113,5);
    display(circle112,5);
    display(circle111,5);
    display(circle110,5);
    display(circle109,5);
    display(circle108,5);




    //------------2----------//

    display(circle107,5);
    display(circle106,5);
    display(circle105,5);
    display(circle104,5);
    display(circle103,5);
    display(circle102,5);
    display(circle101,5);
    display(circle100,5);
    display(circle99,5);
    display(circle98,5);
    display(circle97,5);
    display(circle96,5);
    display(circle95,5);
    display(circle94,5);
    display(circle93,5);
    display(circle92,5);
    display(circle91,5);
    display(circle90,5);
    display(circle89,5);
    display(circle88,5);
    display(circle87,5);



    //----------3------------//

    display(circle86,5);
    display(circle85,5);
    display(circle84,5);
    display(circle83,5);
    display(circle82,5);
    display(circle81,5);
    display(circle80,5);
    display(circle79,5);
    display(circle78,5);
    display(circle77,5);
    display(circle76,5);
    display(circle75,5);
    display(circle74,5);


    //----------4-------------//

    display(circle73,5);
    display(circle72,5);
    display(circle71,5);
    display(circle70,5);
    display(circle69,5);

    //---------5-----------//

    display(circle68,5);
    display(circle67,5);
    display(circle66,5);
    display(circle65,5);
    display(circle64,5);

    //---------6-----------//

    display(circle63,5);
    display(circle62,5);
    display(circle61,5);
    display(circle60,5);
    display(circle59,5);
    display(circle58,5);
    display(circle57,5);
    display(circle56,5);
    display(circle55,5);
    display(circle54,5);
    display(circle53,5);
    display(circle52,5);
    display(circle51,5);


    //--------7-----------//

    display(circle50,5);
    display(circle49,5);
    display(circle48,5);
    display(circle47,5);
    display(circle46,5);
    display(circle45,5);
    display(circle44,5);
    display(circle43,5);
    display(circle42,5);
    display(circle41,5);
    display(circle40,5);
    display(circle39,5);
    display(circle38,5);
    display(circle37,5);
    display(circle36,5);
    display(circle35,5);
    display(circle34,5);
    display(circle33,5);
    display(circle32,5);
    display(circle31,5);
    display(circle30,5);


    //------------8----------//

    display(circle29,5);
    display(circle28,5);
    display(circle27,5);
    display(circle26,5);
    display(circle25,5);
    display(circle24,5);
    display(circle23,5);
    display(circle22,5);
    display(circle21,5);
    display(circle20,5);
    display(circle19,5);
    display(circle18,5);
    display(circle17,5);
    display(circle16,5);
    display(circle15,5);
    display(circle14,5);
    display(circle13,5);
    display(circle12,5);
    display(circle11,5);
    display(circle10,5);
    display(circle9,5);
    display(circle8,5);
    display(circle7,5);
    display(circle6,5);
    display(circle5,5);
    display(circle4,5);
    display(circle3,5);
    display(circle2,5);
    display(circle1,5);



    //----------8------------//

    display(circle1,5);
    display(circle2,5);
    display(circle3,5);
    display(circle4,5);
    display(circle5,5);
    display(circle6,5);
    display(circle7,5);
    display(circle8,5);
    display(circle9,5);
    display(circle10,5);
    display(circle11,5);
    display(circle12,5);
    display(circle13,5);
    display(circle14,5);
    display(circle15,5);
    display(circle16,5);
    display(circle17,5);
    display(circle18,5);
    display(circle19,5);
    display(circle20,5);
    display(circle21,5);
    display(circle22,5);
    display(circle23,5);
    display(circle24,5);
    display(circle25,5);
    display(circle26,5);
    display(circle27,5);
    display(circle28,5);
    display(circle29,5);

    //----------7------------//

    display(circle30,5);
    display(circle31,5);
    display(circle32,5);
    display(circle33,5);
    display(circle34,5);
    display(circle35,5);
    display(circle36,5);
    display(circle37,5);
    display(circle38,5);
    display(circle39,5);
    display(circle40,5);
    display(circle41,5);
    display(circle42,5);
    display(circle43,5);
    display(circle44,5);
    display(circle45,5);
    display(circle46,5);
    display(circle47,5);
    display(circle48,5);
    display(circle49,5);
    display(circle50,5);

    //------------6------------//

    display(circle51,5);
    display(circle52,5);
    display(circle53,5);
    display(circle54,5);
    display(circle55,5);
    display(circle56,5);
    display(circle57,5);
    display(circle58,5);
    display(circle59,5);
    display(circle60,5);
    display(circle61,5);
    display(circle62,5);
    display(circle63,5);

    //---------5-----------//

    display(circle64,5);
    display(circle65,5);
    display(circle66,5);
    display(circle67,5);
    display(circle68,5);

    //----------4-------------//

    display(circle69,5);
    display(circle70,5);
    display(circle71,5);
    display(circle72,5);
    display(circle73,5);

    //----------3------------//

    display(circle74,5);
    display(circle75,5);
    display(circle76,5);
    display(circle77,5);
    display(circle78,5);
    display(circle79,5);
    display(circle80,5);
    display(circle81,5);
    display(circle82,5);
    display(circle83,5);
    display(circle84,5);
    display(circle85,5);
    display(circle86,5);

    //------------2----------//

    display(circle87,5);
    display(circle88,5);
    display(circle89,5);
    display(circle90,5);
    display(circle91,5);
    display(circle92,5);
    display(circle93,5);
    display(circle94,5);
    display(circle95,5);
    display(circle96,5);
    display(circle97,5);
    display(circle98,5);
    display(circle99,5);
    display(circle100,5);
    display(circle101,5);
    display(circle102,5);
    display(circle103,5);
    display(circle104,5);
    display(circle105,5);
    display(circle106,5);
    display(circle107,5);

    //----------1--------//

    display(circle108,5);
    display(circle109,5);
    display(circle110,5);
    display(circle111,5);
    display(circle112,5);
    display(circle113,5);
    display(circle114,5);
    display(circle115,5);
    display(circle116,5);
    display(circle117,5);
    display(circle118,5);
    display(circle119,5);
    display(circle120,5);
    display(circle121,5);
    display(circle122,5);
    display(circle123,5);
    display(circle124,5);
    display(circle125,5);
    display(circle126,5);
    display(circle127,5);
    display(circle128,5);
    display(circle129,5);
    display(circle130,5);
    display(circle131,5);
    display(circle132,5);
    display(circle133,5);
    display(circle134,5);
    display(circle135,5);
    display(circle136,5);
    display(circle137,5);
    display(circle138,5);
    display(circle139,5);
    display(circle140,5);
    display(circle141,5);
    display(circle142,5);
}

void circle_a()
{

    display(circle_a1,5);
    display(circle_a2,5);
    display(circle_a3,5);
    display(circle_a4,5);
    display(circle_a5,5);
    display(circle_a6,5);
    display(circle_a7,5);
    display(circle_a8,5);
    display(circle_a9,5);
    display(circle_a10,5);
    display(circle_a11,5);
    display(circle_a12,5);
    display(circle_a13,5);
    display(circle_a14,5);
    display(circle_a15,5);
    display(circle_a16,5);
    display(circle_a17,5);
    display(circle_a18,5);
    display(circle_a19,5);
    display(circle_a20,5);
    display(circle_a21,5);
    display(circle_a22,5);
    display(circle_a23,5);
    display(circle_a24,5);
    display(circle_a25,5);
    display(circle_a26,5);
    display(circle_a27,5);
    display(circle_a28,5);
    display(circle_a29,5);
    display(circle_a30,5);
    display(circle_a31,5);
    display(circle_a32,5);
    display(circle_a33,5);
    display(circle_a34,5);
    display(circle_a35,5);
    display(circle_a36,5);
    display(circle_a37,5);
    display(circle_a38,5);
    display(circle_a39,5);
    display(circle_a40,5);
    display(circle_a41,5);
    display(circle_a42,5);
    display(circle_a43,5);
    display(circle_a44,5);
    display(circle_a45,5);
    display(circle_a46,5);
    display(circle_a47,5);
    display(circle_a48,5);
    display(circle_a49,5);
    display(circle_a50,5);
    display(circle_a51,5);
    display(circle_a52,5);
    display(circle_a53,5);
    display(circle_a54,5);
    display(circle_a55,5);
    display(circle_a56,5);
    display(circle_a57,5);
    display(circle_a58,5);
    display(circle_a59,5);
    display(circle_a60,5);
    display(circle_a61,5);
    display(circle_a62,5);
    display(circle_a63,5);
    display(circle_a64,5);
    display(circle_a65,5);
    display(circle_a66,5);
    display(circle_a67,5);
    display(circle_a68,5);
    display(circle_a69,5);
    display(circle_a70,5);
    display(circle_a71,5);
    display(circle_a72,5);
    display(circle_a73,5);
    display(circle_a74,5);
    display(circle_a75,5);
    display(circle_a76,5);
    display(circle_a77,5);
    display(circle_a78,5);
    display(circle_a79,5);
    display(circle_a80,5);
    display(circle_a81,5);
    display(circle_a82,5);
    display(circle_a83,5);
    display(circle_a84,5);
    display(circle_a85,5);
    display(circle_a86,5);
    display(circle_a87,5);
    display(circle_a88,5);
    display(circle_a89,5);
    display(circle_a90,5);
    display(circle_a91,5);
    display(circle_a92,5);
    display(circle_a93,5);
    display(circle_a94,5);
    display(circle_a95,5);
    display(circle_a96,5);
    display(circle_a97,5);
    display(circle_a98,5);
    display(circle_a99,5);
    display(circle_a100,5);
    display(circle_a101,5);
    display(circle_a102,5);
    display(circle_a103,5);
    display(circle_a104,5);
    display(circle_a105,5);
    display(circle_a106,5);
    display(circle_a107,5);
    display(circle_a108,5);
    display(circle_a109,5);
    display(circle_a110,5);
    display(circle_a111,5);
    display(circle_a112,5);
    display(circle_a113,5);
    display(circle_a114,5);
    display(circle_a115,5);
    display(circle_a116,5);
    display(circle_a117,5);
    display(circle_a118,5);
    display(circle_a119,5);
    display(circle_a120,5);
    display(circle_a121,5);
    display(circle_a122,5);
    display(circle_a123,5);
    display(circle_a124,5);
    display(circle_a125,5);
    display(circle_a126,5);
    display(circle_a127,5);
    display(circle_a128,5);
    display(circle_a129,5);
    display(circle_a130,5);
    display(circle_a131,5);
    display(circle_a132,5);
    display(circle_a133,5);
    display(circle_a134,5);
    display(circle_a135,5);
    display(circle_a136,50);
    display(circle_a135,5);
    display(circle_a134,5);
    display(circle_a133,5);
    display(circle_a132,5);
    display(circle_a131,5);
    display(circle_a130,5);
    display(circle_a129,5);
    display(circle_a128,5);
    display(circle_a127,5);
    display(circle_a126,5);
    display(circle_a125,5);
    display(circle_a124,5);
    display(circle_a123,5);
    display(circle_a122,5);
    display(circle_a121,5);
    display(circle_a120,5);
    display(circle_a119,5);
    display(circle_a118,5);
    display(circle_a117,5);
    display(circle_a116,5);
    display(circle_a115,5);
    display(circle_a114,5);
    display(circle_a113,5);
    display(circle_a112,5);
    display(circle_a111,5);
    display(circle_a110,5);
    display(circle_a109,5);
    display(circle_a108,5);
    display(circle_a107,5);
    display(circle_a106,5);
    display(circle_a105,5);
    display(circle_a104,5);
    display(circle_a103,5);
    display(circle_a102,5);
    display(circle_a101,5);
    display(circle_a100,5);
    display(circle_a99,5);
    display(circle_a98,5);
    display(circle_a97,5);
    display(circle_a96,5);
    display(circle_a95,5);
    display(circle_a94,5);
    display(circle_a93,5);
    display(circle_a92,5);
    display(circle_a91,5);
    display(circle_a90,5);
    display(circle_a89,5);
    display(circle_a88,5);
    display(circle_a87,5);
    display(circle_a86,5);
    display(circle_a85,5);
    display(circle_a84,5);
    display(circle_a83,5);
    display(circle_a82,5);
    display(circle_a81,5);
    display(circle_a80,5);
    display(circle_a79,5);
    display(circle_a78,5);
    display(circle_a77,5);
    display(circle_a76,5);
    display(circle_a75,5);
    display(circle_a74,5);
    display(circle_a73,5);
    display(circle_a72,5);
    display(circle_a71,5);
    display(circle_a70,5);
    display(circle_a69,5);
    display(circle_a68,5);
    display(circle_a67,5);
    display(circle_a66,5);
    display(circle_a65,5);
    display(circle_a64,5);
    display(circle_a63,5);
    display(circle_a62,5);
    display(circle_a61,5);
    display(circle_a60,5);
    display(circle_a59,5);
    display(circle_a58,5);
    display(circle_a57,5);
    display(circle_a56,5);
    display(circle_a55,5);
    display(circle_a54,5);
    display(circle_a53,5);
    display(circle_a52,5);
    display(circle_a51,5);
    display(circle_a50,5);
    display(circle_a49,5);
    display(circle_a48,5);
    display(circle_a47,5);
    display(circle_a46,5);
    display(circle_a45,5);
    display(circle_a44,5);
    display(circle_a43,5);
    display(circle_a42,5);
    display(circle_a41,5);
    display(circle_a40,5);
    display(circle_a39,5);
    display(circle_a38,5);
    display(circle_a37,5);
    display(circle_a36,5);
    display(circle_a35,5);
    display(circle_a34,5);
    display(circle_a33,5);
    display(circle_a32,5);
    display(circle_a31,5);
    display(circle_a30,5);
    display(circle_a29,5);
    display(circle_a28,5);
    display(circle_a27,5);
    display(circle_a26,5);
    display(circle_a25,5);
    display(circle_a24,5);
    display(circle_a23,5);
    display(circle_a22,5);
    display(circle_a21,5);
    display(circle_a20,5);
    display(circle_a19,5);
    display(circle_a18,5);
    display(circle_a17,5);
    display(circle_a16,5);
    display(circle_a15,5);
    display(circle_a14,5);
    display(circle_a13,5);
    display(circle_a12,5);
    display(circle_a11,5);
    display(circle_a10,5);
    display(circle_a9,5);
    display(circle_a8,5);
    display(circle_a7,5);
    display(circle_a6,5);
    display(circle_a5,5);
    display(circle_a4,5);
    display(circle_a3,5);
    display(circle_a2,5);
    display(circle_a1,5);
}

void word()
{

    display(word1,5);
    display(word2,5);
    display(word3,5);
    display(word4,5);
    display(word5,5);
    display(word6,5);
    display(word7,5);
    display(word8,50);
    display(word9,5);
    display(word10,5);
    display(word11,5);
    display(word12,5);
    display(word13,5);
    display(word14,5);
    display(word15,50);
    display(word16,5);
    display(word17,5);
    display(word18,5);
    display(word19,5);
    display(word20,5);
    display(word21,5);
    display(word22,5);
    display(word23,50);
    display(word24,5);
    display(word25,5);
    display(word26,5);
    display(word27,5);
    display(word28,5);
    display(word29,5);
    display(word30,5);
    display(word31,5);
    display(word32,5);
    display(word33,5);
    display(word34,5);
    display(word35,5);
    display(word36,5);
    display(word37,5);
    display(word38,5);
    display(word39,5);
    display(word40,5);
    display(word41,5);
    display(word42,5);
    display(word43,5);
    display(word44,5);
    display(word45,5);
    display(word46,5);
    display(word47,5);
    display(word48,5);
    display(word49,5);
    display(word50,5);
    display(word51,5);
    display(word52,5);
    display(word53,5);
    display(word54,5);
    display(word55,5);
    display(word56,5);
    display(word57,5);
    display(word58,5);
    display(word59,5);
    display(word60,5);
    display(word61,5);
    display(word62,5);
    display(word63,5);
    display(word64,5);
    display(word65,5);
    display(word66,5);
    display(word67,5);
    display(word68,5);
    display(word69,5);
    display(word70,5);
    display(word71,5);
    display(word72,5);
    display(word73,5);
    display(word74,5);
    display(word75,5);
    display(word76,5);
    display(word77,5);
    display(word78,5);
    display(word79,5);
    display(word80,5);
    display(word81,5);
    display(word82,5);
    display(word83,5);
    display(word84,5);
    display(word85,5);
    display(word86,5);
    display(word87,5);
    display(word88,5);
    display(word89,5);
    display(word90,5);
    display(word91,5);
    display(word92,5);
    display(word93,5);
    display(word94,5);
    display(word95,5);
    display(word96,5);
    display(word97,5);
    display(word98,5);
    display(word99,5);
    display(word100,5);
    display(word101,5);
    display(word102,5);
    display(word103,5);
    display(word104,5);
    display(word105,5);
    display(word106,5);
    display(word107,5);
    display(word108,5);
    display(word109,5);
    display(word110,5);
    display(word111,5);
    display(word112,5);
    display(word113,5);
    display(word114,5);
    display(word115,5);
    display(word116,5);
    display(word117,5);
    display(word118,5);
    display(word119,5);
    display(word120,5);
    display(word121,5);
    display(word122,5);
    display(word123,5);
    display(word124,5);
    display(word125,5);
    display(word126,5);
    display(word127,5);
    display(word128,5);
    display(word129,5);
    display(word130,5);
    display(word131,5);
    display(word132,5);
    display(word133,5);
    display(word134,5);
    display(word135,5);
    display(word136,5);
    display(word137,5);
    display(word138,5);
    display(word139,5);
    display(word140,5);
    display(word141,5);
    display(word142,5);
    display(word143,5);
    display(word144,5);
    display(word145,5);
    display(word146,5);
    display(word147,5);
    display(word148,5);
    display(word149,5);
    display(word150,5);
    display(word151,5);
    display(word152,5);
    display(word153,5);
    display(word154,5);
    display(word155,5);
    display(word156,5);
    display(word157,5);
    display(word158,5);
    display(word159,5);
    display(word160,5);
    display(word161,5);
    display(word162,5);
    display(word163,5);
    display(word164,5);
    display(word165,5);
    display(word166,5);
    display(word167,5);
    display(word168,5);
    display(word169,5);
    display(word170,5);
    display(word171,5);
    display(word172,5);
    display(word173,5);
    display(word174,5);
    display(word175,5);
    display(word176,5);
    display(word177,5);
    display(word178,5);
    display(word179,5);
    display(word180,5);
    display(word181,5);
    display(word182,5);
    display(word183,5);
    display(word184,5);
    display(word185,5);
    display(word186,5);
    display(word187,5);
    display(word188,5);
    display(word189,5);
    display(word190,5);
    display(word191,5);
    display(word192,5);
    display(word193,5);
    display(word194,5);
    display(word195,5);
    display(word196,5);
    display(word197,5);
    display(word198,5);
    display(word199,5);
    display(word200,5);
    display(word201,5);
    display(word202,5);
    display(word203,5);
    display(word204,5);
    display(word205,5);
    display(word206,5);
    display(word207,5);
    display(word208,5);
    display(word209,5);
    display(word210,5);
    display(word211,5);
    display(word212,300);
    display(word213,5);
    display(word214,5);
    display(word215,5);
    display(word216,5);
    display(word217,5);
    display(word218,5);
    display(word219,5);
}



