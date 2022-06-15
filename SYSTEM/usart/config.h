#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "stdio.h"
#include "delay.h"
#include "usart.h"
//#include "motor.h"
//#include "pwm.h"

#define __DEBUG__  


#ifdef __DEBUG__  
#define debug(format,...) printf(__FILE__":%04d "format, __LINE__, ##__VA_ARGS__)  
#define debug_raw(format,...) printf(format, ##__VA_ARGS__)  
#else  
#define debug(format,...)  
#define debug_raw(format,...)  
#endif 


#endif /* __CONFIG_H__ */


