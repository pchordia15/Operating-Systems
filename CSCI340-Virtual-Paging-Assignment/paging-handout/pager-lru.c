/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

	/* Local vars */
	int proc;
	int pc;
	int page;
	int oldpage;
	int stick = INT_MAX;

	static int tick = 1;
	static int initialized = 0;
	static int timestamps[MAXPROCESSES][MAXPROCPAGES];

	if (!initialized) {
		for (proc = 0; proc < MAXPROCESSES; proc++) {
			for (page = 0; page < MAXPROCPAGES; page++) {
				timestamps[proc][page] = 0;
			}
		}
		initialized = 1;
	}

	for (proc = 0; proc < MAXPROCESSES; proc++) {
		if (q[proc].active) {
			pc = q[proc].pc; 		        // program counter for process
			page = pc / PAGESIZE;
			timestamps[proc][page] = tick;
			if (!q[proc].pages[page]) {
				if (!pagein(proc, page)) {
					for (oldpage = 0; oldpage < q[proc].npages; oldpage++) {
						if (oldpage != page) {
							if(q[proc].pages[page] == 1 && timestamps[proc][page] < stick){
								timestamps[proc][page] = stick;
								pageout(proc,oldpage);
								continue;
							}
						}
						if(!pageout(proc, oldpage)){
							/* TODO: Implement LRU Paging */
							fprintf(stderr, "pager-lru not yet implemented. Exiting...\n");
							exit(EXIT_FAILURE);
						}
					}
				}
			}
			continue;
		}
	}
	/* advance time for next pageit iteration */
	tick++;
}
