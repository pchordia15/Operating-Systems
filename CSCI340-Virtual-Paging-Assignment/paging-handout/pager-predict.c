/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "simulator.h"

#define FLAG -1

struct pstat
{
	int page;
	int freq;
	int *timestamp;
};

typedef struct  pstat stat;

int lru(int timestamps[MAXPROCESSES][MAXPROCPAGES], Pentry q[MAXPROCESSES], int proc, int *lpage){
	int i;
	int stick = INT_MAX;
	int count = 1;
	i = 0;
	while(i < MAXPROCPAGES){
		if(q[proc].pages[i] == 1 && timestamps[proc][i] < stick)
		{
			*lpage = i;
		}
		i++;
	}
	return count;
}

void init(stat computepredict[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES]){
	int i, j, k;
	i = 0;
	while(i<MAXPROCESSES){
		j = 0;
		while(j<MAXPROCESSES){
			k = 0;
			while(k<MAXPROCESSES){
				computepredict[i][j][k].freq = FLAG;
				computepredict[i][j][k].timestamp = NULL;
				computepredict[i][j][k].page = FLAG;
				k++;
			}
			j++;
		}
		i++;
	}
}

void insert( int cpage,  int proc, int lpage, stat computepredict[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES], int timestamps[MAXPROCESSES][MAXPROCPAGES]){
	int i;
	stat *block;
	block = computepredict[proc][lpage];
	i = 0;
	while(i<MAXPROCPAGES){
		if(block[i].page == FLAG){
			block[i].freq = 1;
			block[i].page = cpage;
			block[i].timestamp = &(timestamps[proc][i]);
			break;
		}
		if(block[i].page == cpage){
			block[i].freq++;
			break;
		}
		i++;
	}
}

int pcount(stat *next){
	int length;
	for(length = 0; next[length].page != FLAG && length<MAXPROCPAGES; length++){
	}
	return length;
}

stat* predict(int proc, stat computepredict[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES], int cur_pc){
	return computepredict[proc][(cur_pc + 100)/PAGESIZE];
}

void sort(stat *next){
	int length = pcount(next);
	int i;
	int swap = 0;
	stat temp;
	do{
		swap=0;
		i=1;
		while(i<length){
			if(*(next[i-1].timestamp) < *(next[i].timestamp)){
				temp = *(next+(i-1));
				*(next+(i-1)) = *(next+i);
				*(next+i) = temp;
				swap = 1;
			}
			i++;
		}
	}while(swap);
}

void pageit(Pentry q[MAXPROCESSES]){

	/* This file contains the stub for a predictive pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time
	static int timestamps[MAXPROCESSES][MAXPROCPAGES];
	static int pc_last[MAXPROCESSES];

	static stat computepredict[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES];

	/* Local vars */
	int proctmp;
	int pagetmp;
	int lru_page;
	int lpage;
	int cpage;


	/* initialize static vars on first run */
	if (!initialized) {
		init(computepredict);
		proctmp = 0;
		while(proctmp < MAXPROCESSES){
			pagetmp = 0;
			while(pagetmp < MAXPROCPAGES){
				timestamps[proctmp][pagetmp] = 0;
				pagetmp++;
			}
			proctmp++;
		}
		initialized = 1;
	}

	proctmp = 0;
	while(proctmp<MAXPROCESSES){
		if(q[proctmp].active){
			if(lpage != -1){
				lpage = (pc_last[proctmp])/PAGESIZE;
				pc_last[proctmp] = q[proctmp].pc;
				cpage = (q[proctmp].pc)/PAGESIZE;
				if(lpage != cpage){
					pageout(proctmp, lpage);
					insert(cpage, proctmp, lpage, computepredict, timestamps);
				}
			}
		}
		proctmp++;
	}

	proctmp = 0;
	while(proctmp<MAXPROCESSES){
		stat *prediction;
		int i;
		if(q[proctmp].active){
			prediction = predict(proctmp, computepredict, q[proctmp].pc);
			sort(prediction);
			i = 0;
			while(i<pcount(prediction)){
				pagein(proctmp, prediction[i].page);
				i++;
			}
		}
		proctmp++;
	}

	for(proctmp=0; proctmp<MAXPROCESSES; proctmp++){
		if (!q[proctmp].active){
			pagetmp = 0;
			while(pagetmp<MAXPROCPAGES){
				pageout(proctmp,pagetmp);
				pagetmp++;
			}
			continue;
		}
		pagetmp = (q[proctmp].pc)/PAGESIZE;
		if(q[proctmp].pages[pagetmp] != 1){
			if(pagein(proctmp,pagetmp)){
				continue;
			}
		}

		if(lru(timestamps, q, proctmp, &lru_page)){
			continue;
		}
		if(!pageout(proctmp, lru_page)){
			/* TODO: Implement Predictive Paging */
			fprintf(stderr, "pager-predict not yet implemented. Exiting...\n");
			exit(EXIT_FAILURE);
		}
	}
	/* advance time for next pageit iteration */
	tick++;
}
