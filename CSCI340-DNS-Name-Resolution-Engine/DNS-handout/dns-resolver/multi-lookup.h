/*
 * File: multi-lookup.h
 * Author: Priyanka Chordia
 * Project: CSCI340
 * Description:
 * 	This file contains the
 *      header file to DNS resolver.
 *
 */


#ifndef MULTILOOKUP_H
#define MULTILOOKUP_H

#include "util.h"

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
#define MIN_RESOLVER_THREADS 2
#define QUEUE_SIZE 10
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

void* Requester(void* fName);
void* Resolver(void* fName);

#endif
