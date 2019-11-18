
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/ipc.h> 
#include <sys/shm.h>

int BUFFER = 0;

#define MUTEX "mysem"
#define EMPTY "mysem2"
#define FULL "mysem3"


/*
Rene Figueroa
Programming Project 5 - Consumer and Producer  
CMPE 142
*/

/* Sample runs:

producer-consumer -p -m "hello world" -q 100 -e -> 7 args
prodcuer-consumer -c -e -> 3 args


rene@rene-VirtualBox:~/Documents/PP5$ gcc consumer_producer.c -o test -lpthread
rene@rene-VirtualBox:~/Documents/PP5$ ./test -p -m "hello, world rene alfredo figueroa corado" -q 1000 -e

hello, world rene alfredo figueroa corado
rene@rene-VirtualBox:~/Documents/PP5$ ./test -c -e

hello, world rene alfredo figueroa corado
rene@rene-VirtualBox:~/Documents/PP5$ ./test -p -m "hello, world rene alfredo figueroa corado" -q 1000 
rene@rene-VirtualBox:~/Documents/PP5$ ./test -c -e

hello, world rene alfredo figueroa corado
rene@rene-VirtualBox:~/Documents/PP5$ 

*/

sem_t empty;
void consumer(bool print);
void producer(char* msg, bool print);


bool display = false;

int main(int argc, char* argv[]) {
sem_t *mysem = sem_open(MUTEX, O_CREAT, 0644, 1);
sem_t *mysem2 = sem_open(FULL, O_CREAT, 0644, 0);

	
	if (argc <= 7) {
		

		if (strcmp(argv[1], "-p") == 0){
			BUFFER = atoi(argv[5]);
			char* message = argv[3];
			sem_t *mysem3 = sem_open(EMPTY, O_CREAT, 0644, BUFFER);
			if (argc == 7 && (strcmp(argv[6], "-e")== 0)) {
				display = true;
				producer(message, display);
			} else {
				producer(message, display);
			}
		} else if (strcmp(argv[1], "-c") == 0) {
			if (argc == 3 && (strcmp(argv[2], "-e") == 0)) {
				display = true;
				consumer(display);
			} else {
			consumer(display);
			}
			
		} else {
			
			fprintf(stderr, "\tSecond argument needs to be -c or -p\n");

		}
	
	} else {
		fprintf(stderr, "\tWrong number of arguments. \n");
	}

	return 0;
}



void producer(char* msg, bool print) {

	sem_t *full_sem = sem_open(FULL, 0);
	sem_t *mutex_sem = sem_open(MUTEX, 0);
	sem_t *empty_sem = sem_open(EMPTY, 0);

	int mutex_value = 0;
	sem_getvalue(mutex_sem, &mutex_value);

	int empty_value = 0;
	sem_getvalue(empty_sem, &empty_value);

	if (mutex_value == 1 && empty_value != 0) {
	    
	    sem_wait(mutex_sem);
	    sem_post(full_sem);

	    // critical section
	    key_t key = ftok("shmfile",65); 
	    int shmid = shmget(key,1024,0666|IPC_CREAT); 
	    char *str = (char*) shmat(shmid,(void*)0,0); 
	  	// copy msg to shared memory
	    strcpy(str,msg); 
	    if (print == true) {
	    	printf("\n%s\n", str);
	    }

	    shmdt(str);

	    sem_wait(empty_sem);
	    sem_post(mutex_sem);
	    
	}
}

void consumer(bool print) {


	sem_t *sem_mutex = sem_open(MUTEX, 0);
	int mutex_value = 0;
	sem_getvalue(sem_mutex, &mutex_value);

	sem_t *sem_full = sem_open(FULL, 0);
	int full_value = 0;
	sem_getvalue(sem_full, &full_value);

	sem_t *sem_empty = sem_open(EMPTY, 0);


	if (mutex_value == 1 && full_value != 0) {
	
		sem_wait(sem_mutex);
		sem_wait(sem_full);


	    key_t key = ftok("shmfile",65); 
	    int shmid = shmget(key,1024,0666|IPC_CREAT); 
	    char *str = (char*) shmat(shmid,(void*)0,0); 
	    if (display == true) {
	    printf("\n%s\n",str); 
		}
		
	      
	    //detach from shared memory  
	    shmdt(str); 
	    
	    // destroy the shared memory 
	    shmctl(shmid,IPC_RMID,NULL); 

	    sem_post(sem_empty);
	    sem_post(sem_mutex);
	}
}


