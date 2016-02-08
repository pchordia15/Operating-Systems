/*
   Priyanka Chordia.
   Turn in id:- pchordia.
 */


// insert at the end and to delete do it from front.



#include "schedule.h"
#include <stdlib.h>

struct node{
	int value;
	struct node *next;
}*front, *rear, *temp;


/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){
	front = rear = NULL;
}

/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid){

	//enqueue
	if(rear == NULL)
	{
		rear = (struct node*)malloc(sizeof(struct node));
		rear->next = NULL;
		rear->value = pid;
		front = rear;
	}
	else
	{
		temp = (struct node*)malloc(sizeof(struct node));
		rear->next = temp;
		temp->value = pid;
		temp->next = NULL;
		rear = temp;
	}
	return 0;
}

/**
 * Function to get the next process from the scheduler
 *
 * @Return returns the thread id of the next process that should be
 *      executed, returns -1 if there are no processes
 */
int nextProcess(){

	//dequeue
	temp = front;
	if(temp->next != NULL)
	{
		int pid = temp->value;
		temp = temp->next;
		free(front);
		front = temp;
		return pid;
	}
	else
	{
		int pid = temp->value;
		free(front);
		front = NULL;
		rear = NULL;
		return pid;
	}
	return -1;

}

/**
 * Function that returns a boolean 1 True/0 False based on if there are any
 * processes still scheduled
 * @Return 1 if there are processes still scheduled 0 if there are no more
 *		scheduled processes
 */
int hasProcess(){
	if(front == NULL)
		return 0;
	else
		return 1;
}
