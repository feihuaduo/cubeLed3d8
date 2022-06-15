#include <queue.h>
#include <config.h>
#include <stdio.h>

static char queue_buf[QUEUE_SIZE][QUEUE_MSG_LEN]={0};
static signed char front = 0;
static signed char rear = 0;

static char gateway_queue_buf[GATEWAY_QUEUE_SIZE][GATEWAY_QUEUE_MSG_LEN]={0};
static signed char gateway_front = 0;
static signed char gateway_rear = 0;

int put_queue(char *data)
{
	if ((rear+1)%QUEUE_SIZE==front){
		//debug("queue is full!\n");
		return -1;
	}
	if(strlen(data) >= QUEUE_MSG_LEN){
		strncpy(queue_buf[rear],data,QUEUE_MSG_LEN-1);	
		queue_buf[rear][QUEUE_MSG_LEN-1]=0;
	}else
		strcpy(queue_buf[rear],data);
	rear = (rear+1)%QUEUE_SIZE;//此处必须取模
	return 0;
}
int get_queue(char *data)
{
	if (rear==front){
		//debug("queue is empty!\n");
		return -1;
	}
	strcpy(data,queue_buf[front]);
	front = (front+1)%QUEUE_SIZE;//此处必须取模
	return 0;
}

int gateway_put_queue(char *data)
{
	if ((gateway_rear+1)%GATEWAY_QUEUE_SIZE==gateway_front){
		//debug("queue is full!\n");
		return -1;
	}
	if(strlen(data) >= GATEWAY_QUEUE_MSG_LEN){
		strncpy(gateway_queue_buf[gateway_rear],data,GATEWAY_QUEUE_MSG_LEN-1);	
		gateway_queue_buf[gateway_rear][GATEWAY_QUEUE_MSG_LEN-1]=0;
	}else
		strcpy(gateway_queue_buf[gateway_rear],data);
	gateway_rear = (gateway_rear+1)%GATEWAY_QUEUE_SIZE;//此处必须取模
	return 0;
}
int gateway_get_queue(char *data)
{
	if (gateway_rear==gateway_front){
		//debug("queue is empty!\n");
		return -1;
	}
	strcpy(data,gateway_queue_buf[gateway_front]);
	gateway_front = (gateway_front+1)%GATEWAY_QUEUE_SIZE;//此处必须取模
	return 0;
}

