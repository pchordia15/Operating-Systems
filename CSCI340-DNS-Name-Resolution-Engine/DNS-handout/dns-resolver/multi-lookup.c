/*
 * File: multi-lookup.c
 * Author: Priyanka Chordia
 * Project: CSCI340
 * Description:
 * 	This file contains the multi-threaded
 *      solution to DNS resolver.
 *
 */

//http://stackoverflow.com/questions/7038586/return-code-from-pthread-create-is-11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"
#include "multi-lookup.h"

#include "util.h"

//Global Variables:
queue q;
pthread_mutex_t i_queue;
pthread_mutex_t o_queue;
pthread_mutex_t d_queue;
pthread_cond_t queueNotFullCond = PTHREAD_COND_INITIALIZER;

int openRequesters = 0;

int main(int argc, char* argv[]){

	/* Local Vars */
	FILE* outputfp = NULL;

	/* Check Arguments */
	if(argc < MINARGS){
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
	}

	int num_threads = argc - 2;
	int rc,rd;
	long m,n;
	openRequesters = num_threads;
	int numResolverThreads;
	int numCPU = sysconf(_SC_NPROCESSORS_ONLN);

	numResolverThreads = numCPU >= MIN_RESOLVER_THREADS ? numCPU : MIN_RESOLVER_THREADS;
	pthread_t requesterthreads[num_threads];
	pthread_t resolverthreads[numResolverThreads];

	outputfp = fopen(argv[(argc-1)], "w");

	if(!outputfp){
		perror("Error Opening Output File");
		return EXIT_FAILURE;
	}


	if(queue_init(&q, QUEUE_SIZE) == QUEUE_FAILURE){
		fprintf(stderr, "Error: Queue initialization failed!\n");
	}
	pthread_mutex_init(&i_queue, NULL);
	pthread_mutex_init(&o_queue, NULL);
	pthread_mutex_init(&d_queue, NULL);

	m=0;
	while(m<num_threads){
		rc = pthread_create(&(requesterthreads[m]), NULL, Requester, (void*)argv[m+1]);

		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(EXIT_FAILURE);
		}
		m++;
	}

	n=0;
	while(n<numResolverThreads){
		rd = pthread_create(&(resolverthreads[n]), NULL, Resolver, (void*)&outputfp);

		if (rd){
			printf("ERROR; return code from pthread_create() is %d\n", rd);
			exit(EXIT_FAILURE);
		}
		n++;
	}

	m=0;
	while(m<num_threads){
		pthread_join(requesterthreads[m],NULL);
		m++;
	}

	n=0;
	while(n<numResolverThreads){
		pthread_join(resolverthreads[n],NULL);
		n++;
	}

	fclose(outputfp);
	queue_cleanup(&q);
	pthread_mutex_destroy(&i_queue);
	pthread_mutex_destroy(&o_queue);
	pthread_mutex_destroy(&d_queue);

	return EXIT_SUCCESS;
}

void* Requester(void* fName){

	FILE* inputfp;
	char hostname[SBUFSIZE];
	char errorstr[SBUFSIZE];

	inputfp = fopen(fName, "r");

	if(!inputfp){
		sprintf(errorstr, "Error Opening Input File: %s", (char*)fName);
		perror(errorstr);
		printf("Error Opening Input File: %s\n", (char*)fName);
		pthread_mutex_lock(&d_queue);
		openRequesters--;
		pthread_mutex_unlock(&d_queue);
		return NULL;
	}

	while(fscanf(inputfp, INPUTFS, hostname) > 0){
		pthread_mutex_lock(&i_queue);

		while(queue_is_full(&q))
		{
			pthread_cond_wait(&queueNotFullCond, &i_queue);
		}

		if(!queue_is_full(&q)){
			char* hostP = malloc(sizeof(hostname));
			strcpy(hostP, hostname);
			if(queue_push(&q, hostP) == QUEUE_FAILURE){
				fprintf(stderr, "queue push fail \n");
			}
			hostP = NULL;
		}
		pthread_mutex_unlock(&i_queue);
	}

	fclose(inputfp);
	pthread_mutex_lock(&d_queue);
	openRequesters--;
	pthread_mutex_unlock(&d_queue);
	return NULL;
}

void* Resolver(void* fName){

	FILE** outputfp =  fName;

	char* hostP = NULL;
	char hostname[SBUFSIZE];
	char ipstr[MAX_IP_LENGTH];
	int queueIsEmpty = 1;
	node* head_ptr;
	node* tmp_ptr;

	while(openRequesters > 0 || !queueIsEmpty){
		pthread_mutex_lock(&i_queue);
		pthread_cond_signal(&queueNotFullCond);
		if(!(queueIsEmpty = queue_is_empty(&q))){
			if((hostP = queue_pop(&q)) == NULL){
				sprintf(hostname, "%s", (char*) hostP);
			}
		}
		else{
			pthread_mutex_unlock(&i_queue);
			continue;
		}

		pthread_mutex_unlock(&i_queue);

		head_ptr = malloc(sizeof(node));
		head_ptr->link = NULL;

		/* Lookup hostname and get IP string */
		if(multidnslookup(hostP, head_ptr, sizeof(ipstr))
				== UTIL_FAILURE){
			fprintf(stderr, "dnslookup error: %s\n", hostP);
			strncpy(ipstr, "", sizeof(ipstr));
		}

		pthread_mutex_lock(&o_queue);
		fprintf(*outputfp, "%s", hostP);
		while(head_ptr->link != NULL){

			fprintf(*outputfp, ",%s", head_ptr->data);
			tmp_ptr = head_ptr;
			head_ptr = head_ptr->link;
			free(tmp_ptr);
		}
		fprintf(*outputfp, "\n");

		free(hostP);
		free(head_ptr);

		pthread_mutex_unlock(&o_queue);
	}
	return NULL;
}
