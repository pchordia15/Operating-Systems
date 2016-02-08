#Benchmarking Signals and Pipes
Adopted from assignment by Fred Kuhns, Washington University in St. Louis[1]

##Introduction
The purpose of this assignment is for you to become more familiar with the concepts of interprocess communication (IPC). You’ll do this by writing a simple program to benchmark the round-trip time of IPC using signals and Unix pipes.

##Logistics
The only “hand-in” will be electronic. Any clarifications and revisions to the assignment will be posted on my web page and emailed out to the class.

##Hand Out Instructions
I recommend you use an Ubuntu Linux virtual machine to complete this assignment. Alternatively, you can use jaguar or a native Linux installation.

To get the files I recommend you clone the repository to start:

<pre>
git clone git@github.com:CSUChico-CSCI340/CSCI340-IPC-Benchmark.git
</pre>

Then go into the IPC Lab Handout directory

Enter your name in the header comment at the top of *ipc.cc*

Looking at the *ipc.cc* file, you will see that it contains a skeleton and some variables to get you started. To help you get started, I provided the wrapper for the signal handler from your shell assignment, which is in the *helper_routines.cc* file. You will only need to edit the *ipc.cc* file for this assignment.

##General Overview of Pipes
Hopefully you are already familiar with signals from the shell lab. The Unix *pipe()* system call asks the operating system to construct a new anonymous pipe object. This results in two new, opened file descriptors in the process: the read-only end of the pipe, and the write-only end. The pipe ends behave like normal, anonymous file descriptors, except that they have no ability to seek.[2]

To avoid deadlock and exploit parallelism, a Unix process with one or more new pipes will generally call *fork()* to create new processes. Each process will then close the end(s) of the pipe that it will not be using before producing or consuming any data. Alternatively, a process might create a new thread and use the pipe to communicate between them.[2]

You can have more than one pipe in a process, but each pipe is its own anonymous file descriptor. Here is a quick example of the creation of a pipe:

int fd[2];
pipe(fd);

Once the pipe is initialized, you can operate on *fd* like a C file descriptor where *fd[0]* would be the input pipe and *fd[1]* would be the output pipe ”file” descriptor. Here’s a quick example of how you could send the message *"Hello World"* to the parent from the child[3]:

<pre>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int fd[2], nbytes;
	pid_t childpid;
	char string[] = "Hello, world!\n";
	char readbuffer[80];
	pipe(fd);
	if((childpid = fork()) == -1)
	{
		perror("fork");
		exit(1);
	}
	if(childpid == 0)
	{
		// Child process closes up input side of pipe
		close(fd[0]);
		//Send "string" through the output side of pipe
		write(fd[1], string, (strlen(string)+1));
		exit(0);
	}
	else
	{
		//Parent process closes up output side of pipe
		close(fd[1]);
		//Read in a string from the pipe
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		printf("Received string: %s", readbuffer);
	}
	return(0);
}
</pre>

##Your Task
Your task for this assignment is to measure the amount of time it takes for a message to come back once a message has been sent from parent to child or child to parent. That is, your program should measure the round-trip time between parent and child processes. Your program should measure wall-clock time, not CPU time.

After making the specified number of time measurements, your program should output the average, min, and max round-trip times, and the total elapsed running time. You’ll also need your parent and child process to identify themselves as the parent or child and display their process id and group id. Here is the expected output for a default run of the program benchmarking pipes:

<pre>
bash$ ./ipc -p
Number of Tests 10000
Child’s Results for Pipe IPC mechanisms
Process ID is 4776, Group ID is 1000
Round trip times
Average 0.003187
Maximum 0.126000
Minimum 0.002000
Elapsed Time 32.324000
Parent’s Results for Pipe IPC mechanisms
Process ID is 4775, Group ID is 1000
Round trip times
Average 0.003128
Maximum 0.127000
Minimum 0.002000
Elapsed Time 32.317000
</pre>

##Program Specification
Your program should take the following arguments:

* -p this argument should cause your program to benchmark pipes
* -s this argument should cause your program to benchmark signals
* [number] this argument is the number of tests you want to run. It is optional; if no value is specified, 10000 tests will be run.

##Checking Your Work
I have provided some tools to help you check your work:

* Reference solution. As in the shell assignment, a reference program has been included, *ipcref*, which is a fully functional version of this assignment. *Your program should emit output that is identical to the reference solution.*
* Makefile. The Makefile takes arguments that allow you to test the default number of tests for pipes or signals. To test pipes messaging, type *make pipes* for signals type *make signals*.

##Hints

Read the references links of this document on pipes.

It is often helpful to run your program for a single iteration instead of many iterations.

It is often helpful to trace the flow of your program and verify variable state by generating temporary debug output.

Remove any temporary debug output before submitting your assignment! The output of your final submission must match the reference output exactly.


##Evaluation
Your solution will be tested to make sure that it performs the requested behavior of correctly measuring the round-trip times for both signals and pipes. Your solution will also be tested to see that it outputs in the same format as the reference program. If your program accomplishes both of these things it will be considered correct.

##Hand In Instructions
You only have to change *ipc.cc*. You need to upload *ipc.cc* to the http://turnin.ecst.csuchico.edu/ page to mark your completion time.

##References
[1] Fred Kuhns *CS422: Operating Systems Organization*. Washington University in St. Louis: Spring 2004. http://www.cs.wustl.edu/ ̃fredk/Courses/cse422/sp04/Projects/Project1.html.

[2] Wikipedia. “Pipeline (Unix)”. Wikipedia, The Free Encyclopedia. 2012.
http://en.wikipedia.org/wiki/Pipeline_%28Unix%29. Online; accessed 2-February-2014.

[3] The Linux Documentation Project “6.2.2 Creating Pipes in C)”.
http://www.tldp.org/LDP/lpg/node11.html. Online; accessed 2-February-2014.
