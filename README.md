本程序的开发环境为Keil uVsion5，使用的芯片为STM32F1047ZGT6,该芯片拥有1M的FLASH，能够满足光立方显示的文字、图形、动画等数据的存储。光立方的每颗led灯使用层共阴，列共阳的方法连接，采用8片74HC573分别与每列引出的阳极连接，以此实现引脚复用与光立方每版的控制；每层阴极与ULN2803芯片连接，实现每层的控制。

程序主要分为两个部分：引脚赋值与显示数据扫描。

**一、引脚赋值**

由于使用的芯片是STM32，该芯片并不能像单片机一样直接给一组引脚赋一个字节来改变这组引脚的状态，为方便实现对每组引脚（每版、每层、每列）的控制，让代码复用提高效率，故需要对引脚赋值的方法进行重写封装。一般对STM32引脚赋值是使用GPIO_SetBits与GPIO_SetBits函数进行置1与清0操作，函数要求两个参数，分别是引脚组号与引脚号，其中引脚号是非数字参数，在使用中传参麻烦，故我们在封装时首先将引脚号定义为数字，便于外部使用for等方法调用传参；同时要求传参整形变量bits，根据bits的值来决定是置1还是清0，从而将两个函数简化为一个。

具体实现如下：
```c
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
```

对引脚赋值的函数封装好后，可在此基础上，分别写出设置版、层、列的数据的函数。

思路如下：

定义一个全局的整型数组变量，该数组为引脚编号集合，内容包含该组控制所使用的引脚；函数要求外部传入一个字节，将该字节的8位二进制分别对应数组中的8个引脚赋值，即实现对该组引脚的控制。

示例如下（层控制）：
```c
#define LAYER_GPIO GPIOD
u8 layerPin[]={0,1,2,3,4,5,6,7};

void setLayer(u8 data){
	int i;
	for(i=0;i<8;i++){
		setPin(LAYER_GPIO,layerPin[i],data&(0x01<<i));
	}
}
```
这里层控制中，也就是ULN2803的8个输入端分别连接的是GPIOD_0到GPIO_7共8个引脚，函数接收一个字节数据，在for循环中分别把这个字节的每一位赋值给对应的引脚。这里取值的时候使用到了位运算，最开始保留字节的第0位，其余位相与清0，即相当于取出这位的值，如果最后计算结果等于0，就将引脚清0，否则置1，后面循环相与的数依次左移，即依次向左取位，最终取出所有位。剩下的版和列控制同理。

**二、数据扫描**

数据扫描即将储存在数组中的图像数据依次取出，对相应的版、层、列的引脚进行控制，最终使对应的LED灯点亮。

图像数组为一个含有64个字节元素的一维字节类型数组，为方便扫描与可读，一般将其看作一个含有8个一维数组的二维数组，每个一维数组中有8个字节元素。每个一维数组即一版的的数据，该一维数组中的8个元素分别为该版LED灯所在的8层的数据，一个字节元素中的8位对应该版，该层中相应列的数据，通过上述三次选择即确定了一个字节数据具体点亮的LED灯位置。

整个光立方的坐标系如下图：
![坐标体系](https://github.com/feihuaduo/cubeLed3d8/blob/master/pic/光立方坐标体系.png)


确定了图像数组的储存方式与光立方的坐标轴，即可以确定扫描的方式：从（0，0，0）处开始，依次向（0，1，0）方向扫描到（0，7，0）；然后往上一层，从（0，0，1）开始，重复第一层的扫描步骤，该层扫描完后，重复每层的扫描步骤继续扫描，直到（0，7，7）；最顶层扫描完后，第0版的所有LED灯即都已完成扫描，从第一版的（1，0，0）开始，继续重复第一版的扫描步骤，直到扫描到最后一版的（7，7，7），即所有层都已扫描完毕，此时整个光立方扫描完毕。

**故扫描步骤为：**

1、依次扫描该版该层的所有LED

2、依次扫描该版的所有层

3、依次扫描所有版

示例代码如图：
```c
void display(char dat[][8],int go_speed){
    char speed;
    char x_line;
    char z_line;
    speed=go_speed*7;
    while(speed--){
        for(x_line=0; x_line<8; x_line++){
            setLatchNum(x_line);
            for(z_line=0; z_line<8; z_line++){
                setLayerNum(z_line);
               setData(dat[x_line][z_line]);
            }
        }
    }
}
```



在该扫描函数中，要求传入两个参数，第一个是图像数据的数组，第二个为显示的帧，即一个图像显示的时间，以此便可以控制图像显示的快慢，从而将多个图像数据组合成动画。

在扫面函数的循环中，先是确定了扫描的版，然后在该版中，从第0层开始依次向上扫描，每层扫描时，将数组中对应的值赋给该版该层中的8个LED灯。

通过以上几个函数的封装，整个程序框架基本搭好，对STM32使用到的引脚初始化之后，便可以使用扫描函数对用取模软件生成的图像数组进行扫描显示，最终通过多个图像按照一定顺序，一定时间的显示，构成一个动画。
