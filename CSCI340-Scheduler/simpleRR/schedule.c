/* 
   Priyanka Chordia.
   Turn in id:- pchordia.
 */

#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>

struct node
{
	int value;
	struct node *next;
} *front, *rear, *temp, *temp1;
/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){
	rear = front = NULL;
	temp = temp1 = NULL;
}

/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid){
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
 * Function to remove a process from the scheduler queue
 * @Param pid - the ID for the process/thread to be removed from the
 *      scheduler queue
 * @Return true/false response for if the removal was successful
 */
int removeProcess(int pid){
	if(front!= NULL && front->value == pid) // first in the queue
	{
		temp = front;
		front = front->next;
		free(temp);
		temp = NULL;
		return 1;
	}
	else if(front != NULL)
	{
		temp = front;
		temp1 = temp->next;
		while(temp1 && temp1->value != pid)
		{
			temp = temp1;
			temp1 = temp1->next;
		}
		if(temp1 != NULL)
		{
			temp->next = temp1->next;
			free(temp1);
			temp1 = NULL;
		}
		return 1;
	}
	else
	{
		free(front);
		return 1;
	}
	return 0;
}
/**
 * Function to get the next process from the scheduler
 * @Param time - pass by pointer variable to store the quanta of time
 * 		the scheduled process should run for
 * @Return returns the thread id of the next process that should be
 *      executed, returns -1 if there are no processes
 */
int nextProcess(int *time)
{
	*time = 4;
	if(front != NULL && time)
	{
		int pid = front->value;
		removeProcess(pid);
		addProcess(pid);
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
	if(front != NULL)
		return 1;
	else
		return 0;
}
