
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>


/*
Rene Figueroa
Programming Project 4 - Pthreads 
CMPE 142
*/

// Thread functions 

// Declaration of thread condition variable 
pthread_cond_t condition = PTHREAD_COND_INITIALIZER; 
  
// declaring mutex 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

int flag = 0;
bool EXIT = false;

void signalhandler(int num){
	// SIGINT -> 2
	if (num == 2) {
		//exit(num);
    EXIT = true;

		// signal (num, SIG_DFL);
  // 		raise (num);
		
	}
	

}
/*thread function definition*/
void * thread_msg_1(void* spare)
{
    while(EXIT != true)
    {
       sleep(2);



       pthread_mutex_lock(&lock);
       fprintf(stdout,"thread 1: ping thread 2\n");
       //printf("thread 1: ping thread 2\n");

       flag += 1;
       // send signal to other thread
       pthread_cond_signal(&condition);
       pthread_cond_wait(&condition, &lock);

       pthread_mutex_unlock(&lock);
       if (flag == 0) {
       	// upon receiving the signal from thread2, then print the other message
       	pthread_mutex_lock(&lock);
       	fprintf(stdout, "thread 1: pong! thread 2 ping received\n");
       	pthread_mutex_unlock(&lock);
       }





    }
    return NULL;
}
void * thread_msg_2(void* threadID)
{
    while(EXIT != true)
    {
       pthread_mutex_lock(&lock);
       if(flag < 1)
       {
           pthread_cond_wait(&condition, &lock);
       }
       if(flag > 0)
       {
       		fprintf(stdout,"thread 2: pong! thread 1 ping received\n");
       		fprintf(stdout, "thread 2: ping thread 1\n");
           
           flag--;
           // send cond signal to thread1
           pthread_cond_signal(&condition);
       }
       pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
	signal(SIGINT,signalhandler);

   	 //printf("My process ID is %d\n", getpid());

   

    pthread_t thread_0;
    pthread_t thread_1;

    int thread_0_status = 1;
    int thread_1_status = 1;

    // printf("In main \nthread_0 id = %d\n", thread_0);
    // printf("In main \nthread_1 id = %d\n", thread_1);
  
    thread_0_status = pthread_create(&thread_0, NULL, thread_msg_2, NULL);
    thread_1_status = pthread_create(&thread_1, NULL, thread_msg_1, NULL);
    // if (thread_0_status == 0 && thread_1_status == 0) {
    // 	printf("Threads 0 and 1 were succesfully created\n");
    // } else {
    // 	printf("Threads are not being created\n");
    // }
    

    pthread_join(thread_0, NULL);
    pthread_join(thread_1, NULL);

    

    return 0;
}