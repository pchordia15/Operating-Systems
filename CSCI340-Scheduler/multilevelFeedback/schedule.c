/*
   Priyanka Chordia.
   Turn in id: pchordia
 */

#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>

struct node
{
	int value;
	int age;
	struct node *next;
} *temp, *temp1;

struct node *front[3];
int i = 0;
/**
 * Function to initialize any global variables for the scheduler.
 */
void init(){
	for(i=0; i<3 ; i++)
	{
		front[i] = NULL;
	}
	temp = temp1 = NULL;
}

/**
 * Function called every simulated time period to provide a mechanism
 * to age the scheduled processes and trigger feedback if needed.
 */
void age(){
	int pid;
	for(i = 0; i <= 2; i++)
	{
		temp = front[i];
		while(temp != NULL)
		{
			temp->age++;
			if(i>0 && temp->age == 1000)
			{
				pid = temp -> value;
				removeProcess(pid);
				addProcess(pid, --i);
			}
			temp = temp->next;
		}
	}
}

/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @Param priority - priority of the process being added
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid, int priority){
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
	for(i = 0; i<3; i++)
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
 * 		the scheduled process should run for, returns -1 if should run to completion.
 * @Return returns the thread id of the next process that should be
 *      executed, returns -1 if there are no processes
 */
int nextProcess(int *time){
	int pid;
	if(front[0] != NULL)
	{
		pid = front[0]->value;
		removeProcess(pid);
		return pid;
	}
	else if(front[1] != NULL)
	{
		*time = 4;
		pid = front[1]->value;
		removeProcess(pid);
		addProcess(pid,1);
		return pid;
	}
	else if(front[2] != NULL)
	{
		*time = 1;
		pid = front[2]->value;
		removeProcess(pid);
		addProcess(pid,2);
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
	for(i=0; i<3; i++)
	{
		if(front[i] != NULL)
			return 1;
	}
	return 0;
}
