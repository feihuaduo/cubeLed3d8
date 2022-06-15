#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <string.h>

//������Ϣ
#define QUEUE_SIZE 			3//ʵ�ʴ洢������QUEUE_SIZE-1��
#define QUEUE_MSG_LEN		30//��Ϣ�ַ�������

//NB��Ϣ
#define GATEWAY_QUEUE_SIZE 			10//ʵ��ͨ�Ŵ洢������GATEWAY_QUEUE_SIZE-1��
#define GATEWAY_QUEUE_MSG_LEN		64//��Ϣ�ַ�������

extern int put_queue(char *data);
extern int get_queue(char *data);
extern int gateway_put_queue(char *data);
extern int gateway_get_queue(char *data);

#endif
