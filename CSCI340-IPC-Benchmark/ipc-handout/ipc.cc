//
//
//
// Priyanka Chordia: pchordia
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <limits.h>
#include <sys/time.h>

#include "helper-routines.h"

/*Define Global Variables*/
pid_t   childpid;
timeval t1, t2, rt1, rt2;
int numtests;
double elapsedTime;
double minTime, maxTime;
double tripTime, rtTime;
int flag;
bool loop1 = true;
sigset_t sigmask;

void sigusr1_handler(int sig){
	gettimeofday(&rt2, NULL);
	rtTime = (rt2.tv_sec - rt1.tv_sec)*1000.0;
	rtTime += (rt2.tv_usec - rt1.tv_usec)/1000.0;
	tripTime += rtTime;
	if(rtTime > maxTime)
		maxTime = rtTime;
	if(rtTime < minTime || minTime == 0)
		minTime = rtTime;
	gettimeofday(&rt1, NULL);
	kill(getppid(), SIGUSR2);
}

void sigusr2_handler(int sig){
	gettimeofday(&rt2, NULL);
	rtTime = (rt2.tv_sec - rt1.tv_sec)*1000.0;
	rtTime += (rt2.tv_usec - rt1.tv_usec)/1000.0;
	tripTime += rtTime;
	if(rtTime > maxTime)
		maxTime = rtTime;
	if(rtTime < minTime || minTime == 0)
		minTime = rtTime;
	gettimeofday(&rt1, NULL);
	kill(childpid, SIGUSR1);
}

void sigstp_handler(int sig){
	flag = 0;
	loop1 = false;
	exit(0);
}

//
// main - The main routine
//
int main(int argc, char **argv){
	//Initialize Constants here

	//variables for Pipe
	int fd1[2],fd2[2], nbytes;
	//byte size messages to be passed through pipes
	char    childmsg[] = "1";
	char    parentmsg[] = "2";
	char    quitmsg[] = "q";
	char    readbuff;

	pipe(fd1);
	pipe(fd2);

	/*Three Signal Handlers You Might Need
	 *
	 *I'd recommend using one signal to signal parent from child
	 *and a different SIGUSR to signal child from parent
	 */
	Signal(SIGUSR1,  sigusr1_handler); //User Defined Signal 1
	Signal(SIGUSR2,  sigusr2_handler); //User Defined Signal 2
	Signal(SIGTSTP, sigstp_handler);

	//Default Value of Num Tests
	numtests=10000;

	//Determine the number of messages was passed in from command line arguments
	//Replace default numtests w/ the commandline argument if applicable
	if(argc<2){
		printf("Not enough arguments\n");
		exit(0);
	}

	if (argc == 3) {
		numtests = atoi(argv[2]);
	}

	printf("Number of Tests %d\n", numtests);
	// start timer
	gettimeofday(&t1, NULL);
	if(strcmp(argv[1],"-p")==0){
		minTime = 1000.0;
		maxTime = 0.0;
		//code for benchmarking pipes over numtests
		gettimeofday(&rt1, NULL);
		if ((childpid = fork()) == -1) {
			perror("fork");
			exit(0);
		}
		if (childpid == 0) {
			close(fd1[1]);
			close(fd2[0]);

			while ((nbytes = read(fd1[0], &readbuff, 1)) > 0) {
				gettimeofday(&rt2, NULL);
				rtTime = (rt2.tv_sec - rt1.tv_sec) * 1000.0;
				rtTime += (rt2.tv_usec - rt1.tv_usec) / 1000.0;
				tripTime += rtTime;
				if (rtTime > maxTime)
					maxTime = rtTime;
				if (rtTime < minTime)
					minTime = rtTime;
				if (readbuff == 'q')
					break;
				gettimeofday(&rt1, NULL);
				write(fd2[1], &childmsg, 1);
			}
			close(fd1[0]);
			close(fd2[1]);
			printf("Child's Results for Pipe IPC mechanisms\n");
		}
		else {
			close(fd1[0]);
			close(fd2[1]);

			for (int i = 0; i < numtests; ++i) {
				gettimeofday(&rt1, NULL);
				write(fd1[1], &parentmsg, 1);
				nbytes = read(fd2[0], &readbuff, 1);
				gettimeofday(&rt2, NULL);
				rtTime = (rt2.tv_sec - rt1.tv_sec) * 1000.0;
				rtTime += (rt2.tv_usec - rt1.tv_usec) / 1000.0;
				tripTime += rtTime;
				if (rtTime > maxTime)
					maxTime = rtTime;
				if (rtTime < minTime)
					minTime = rtTime;
			}
			write(fd1[1], &quitmsg, 1);
			close(fd1[1]);
			close(fd2[0]);

			wait(NULL);
			printf("Parent's Results for Pipe IPC mechanisms\n");
		}
		printf("Process ID is %d, Group ID is %d\n", getpid(), getgid());
		printf("Round trip times\n");
		printf("Average %f\n", tripTime / numtests);
		printf("Maximum %f\n", maxTime);
		printf("Minimum %f\n", minTime);

		// stop timer
		gettimeofday(&t2, NULL);

		// compute and print the elapsed time in millisec
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
		printf("Elapsed Time %f\n", elapsedTime);
	}

	if(strcmp(argv[1],"-s")==0){
		//code for benchmarking signals over numtests
		int currentTest = 0;
		if ((childpid = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}
		if(childpid == 0)
		{
			maxTime = 0;
			gettimeofday(&rt1, NULL);
				printf("Child's Results for Signal IPC mechanisms \n");
				gettimeofday(&t2, NULL);
				// compute and print the elapsed time in millisec
				elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
				elapsedTime += (t2.tv_usec - t1.tv_usec)/10.0;   // us to ms
				printf("Process ID is %d, Group ID is %d\n", getpid(), getgid());
				printf("Round trip times\n");
				printf("Average %f\n", (elapsedTime / numtests) * 10);
				printf("Maximum %f\n", maxTime*10);
				printf("Minimum %f\n", minTime);
				printf("Elapsed Time %f\n", elapsedTime);
			while(loop1)
			{
				sigsuspend(&sigmask);
			}
		}
		else
		{
			maxTime = 0;
			gettimeofday(&rt1, NULL);
			kill(childpid, SIGUSR1);
			while(currentTest < numtests)
			{
				sigsuspend(&sigmask);
				currentTest++;
			}
			kill(childpid, SIGINT);
			wait(0);
			printf("Parent's Results for Signal IPC mechanisms \n");
		}

		// stop timer
		gettimeofday(&t2, NULL);
		// compute and print the elapsed time in millisec
		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
		elapsedTime += (t2.tv_usec - t1.tv_usec)/1000.0;   // us to ms
		printf("Process ID is %d, Group ID is %d\n", getpid(), getgid());
		printf("Round trip times\n");
		printf("Average %f\n", elapsedTime / numtests);
		printf("Maximum %f\n", maxTime);
		printf("Minimum %f\n", minTime);
		printf("Elapsed Time %f\n", elapsedTime);
		exit(1);
	}
}
