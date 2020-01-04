#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/queue.h>

void *shortterm(void *param);  //short term scheduler prototype
void *longterm(void *param);   //long term scheduler prototype
	
struct process{          //struct with 2 elements: PID and Time
	int pid;
	int executionTime;
}p1;

struct process ready_q[5];     //ready queue with maximum 5 elements
struct process new_q[100];     //new queue with 100 elements
int readySize = 0;	//size of ready queue
int newSize = 100;	//size of new queue

int main()
{
	for(int i = 0; i < 100; i++) {  //populating new queue with 100 elements
		new_q[i] = p1;
		new_q[i].pid = i + 1;   
		new_q[i].executionTime = (rand() % 30) + 1;   //random time between 1 - 30
	}	
	
	pthread_t short_term, long_term; 	//initializing threads
	pthread_attr_t attr; 	
	pthread_attr_init(&attr);  //initializing attributes
	pthread_create(&long_term, &attr, longterm, NULL);  //creating long term scheduler thread
	pthread_create(&short_term, &attr, shortterm, NULL);	// creating short term scheduler thread
	pthread_join(short_term, NULL);  //waiting for threads to finish
	pthread_join(long_term, NULL);	//waiting for threads to finish
}
	
void *shortterm(void *param){
	printf("Short Term scheduler: Start...");
	printf("Action...");
	printf("Stop...\n");
	for(int j = 0; j < 5; j++){   //repeat 5 times
		struct process temp1 = ready_q[readySize - 1];  
		readySize--;   //de-queue from ready queue
		if(temp1.executionTime != 0){  //if time is not already 0
			temp1.executionTime -= 2;   //reduce time by 2
			ready_q[readySize] = temp1;  
			readySize++;   //then en-queue to ready queue
		}
	}
	longterm(NULL);	//invoke longterm scheduler
}

void *longterm(void *param){
	printf("Long Term Scheduler: Start...");
	printf("Action...");
	printf("Stop...\n");
	struct process temp = new_q[newSize - 1];   
	newSize--;	//de-queue from new queue
	if(readySize <= 5){  //as long as there are less than 5 elements...
		ready_q[readySize] = temp;
		readySize++;			//en-queue to ready queue
		shortterm(NULL);   //invoke short term scheduler
	}
}	