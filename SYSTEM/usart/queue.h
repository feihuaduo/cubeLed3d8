#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <string.h>

//主控消息
#define QUEUE_SIZE 			3//实际存储条数是QUEUE_SIZE-1条
#define QUEUE_MSG_LEN		30//消息字符串长度

//NB消息
#define GATEWAY_QUEUE_SIZE 			10//实际通信存储条数是GATEWAY_QUEUE_SIZE-1条
#define GATEWAY_QUEUE_MSG_LEN		64//消息字符串长度

extern int put_queue(char *data);
extern int get_queue(char *data);
extern int gateway_put_queue(char *data);
extern int gateway_get_queue(char *data);

#endif
