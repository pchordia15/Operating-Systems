/*
   Priyanka Chordia.
   Turn in id:- pchordia.
 */

#include "schedule.h"
#include <stdlib.h>


struct node
{
	int value;
	struct node *next;
} *temp, *temp1;

struct node *front[4];
int i = 0;
int myindex = 0;
int flag1 = 0;
/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){
	for(i=0; i < 4; i++)
	{
		front[i] = NULL;
	}
	temp = temp1 = NULL;
}

/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @Param priority - priority of the process being added
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid, int priority){
	priority--;
	if(front[priority] == NULL)
	{
		front[priority] = (struct node*) malloc(sizeof(struct node));
		front[priority]->value = pid;
		front[priority]->next = NULL;
		return 1;
	}
	else
	{
		temp = front[priority];
		while(temp->next)
		{
			temp = temp->next;
		}
		temp->next = (struct node*) malloc(sizeof(struct node));
		temp->next->value = pid;
		temp->next->next = NULL;
		return 1;
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
	for(i = 0; i<4; i++)
	{
		if(front[i] != NULL)
		{
			if(front[i]->value == pid)
			{
				temp = front[i];
				front[i] = front[i]->next;
				free(temp);
				temp = NULL;
				return 1;
			}
			else
			{
				temp = front[i];
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
					return 1;
				}
			}
		}
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
int nextProcess(int *time){
	int pid;
	int flag = 0;
	for(i = 0; i < 4 ; i++)
	{
		if(front[i] != NULL)
			flag++;
	}
	if (flag == 0)
		return -1;
	if (flag1 != flag)
	{
		flag1 = flag;
		myindex = 4 - flag1;
	}
	if (flag > 0)
	{
		int newmyindex = myindex;
		myindex = (myindex + 1) % 4;
		for (;front[myindex] == NULL;)
		{
			myindex++;
		}
		*time = 4 - newmyindex;
		pid = front[newmyindex]->value;
		removeProcess(pid);
		addProcess(pid, newmyindex+1);
		return pid;
	}
	return 0;
}

/**
 * Function that returns a boolean 1 True/0 False based on if there are any
 * processes still scheduled
 * @Return 1 if there are processes still scheduled 0 if there are no more
 *		scheduled processes
 */
int hasProcess(){
	for (i = 0; i < 4; i++)
	{
		if (front[i] != NULL)
		{
			return 1;
		}
	}
	return 0;
}
