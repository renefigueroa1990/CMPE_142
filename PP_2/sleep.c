#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/*
Rene Figueroa
Programming Project 2
CMPE 142

SAMPLE RUN:

rene@rene-VirtualBox:~/Documents/PP_2$ ./sleep
^Citeration number = 2
iteration number = 3
iteration number = 4
iteration number = 5
iteration number = 6
^C^Citeration number = 11
iteration number = 12
iteration number = 13
iteration number = 14
iteration number = 15
iteration number = 16
iteration number = 17
iteration number = 18
iteration number = 19
iteration number = 20
rene@rene-VirtualBox:~/Documents/PP_2$ 

rene@rene-VirtualBox:~/Documents/PP_1$ ps ax | grep sleep
 3317 pts/0    S+     0:00 ./sleep
 3319 pts/1    S+     0:00 grep --color=auto sleep
rene@rene-VirtualBox:~/Documents/PP_1$ kill -10 3317

*/
// global variable to keep track to toggle
bool debugOn = false;


void signalhandler(int num){
	// SIGINT -> 2
	if (num == 2) {
		debugOn = !debugOn;
	} 
	// SIGUSER1 -> 10
	if (num == 10) {
		exit(0);
		
	}	

}

int main(int argc, char *argv[]) {

	signal(SIGINT,signalhandler);
	signal(SIGUSR1, signalhandler);
	
	int count = 0;
	// run an infinite loop and sleep for 2 seconds in it
	while(1) {
		
		count += 1;
		sleep(2);
		if (debugOn) {
			printf("iteration number = %u\n", count);
			
		} else if (!debugOn) {
			// supress debug
		}
	}
	
	return 0;
}




