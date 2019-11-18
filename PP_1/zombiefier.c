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
Programming Project 1
CMPE 142

SAMPLE RUN:

rene@rene-VirtualBox:~/Documents/PP_1$ ./zombies -n 5

rene@rene-VirtualBox:~/Documents/PP_0$ ps ax | grep zombies
10206 pts/0    S+     0:00 ./zombies -n 5
10207 pts/0    Z+     0:00 [zombies] <defunct>
10208 pts/0    Z+     0:00 [zombies] <defunct>
10209 pts/0    Z+     0:00 [zombies] <defunct>
10210 pts/0    Z+     0:00 [zombies] <defunct>
10211 pts/0    Z+     0:00 [zombies] <defunct>
10213 pts/1    S+     0:00 grep --color=auto zombies
rene@rene-VirtualBox:~/Documents/PP_0$ kill -SIGCONT 10206
rene@rene-VirtualBox:~/Documents/PP_0$ ps ax | grep zombies
10216 pts/1    S+     0:00 grep --color=auto zombies




*/



void signalhandler(int num){
	/*empty signal handler*/
}

int main(int argc, char *argv[]) {
	
	int number_of_childs = 0;
	if (argc == 3) {
		
		number_of_childs = atoi(*(argv + 2));
		
		//
	} else {
		fprintf(stderr, "Wrong number of arguments. Correct syntax: ./zombies -n 10\n");
	}

	// When program receives the SIGCONT signal, then zombies should go away from the system
	signal(SIGCONT,signalhandler);

	// user will enter how many zombies he/she wants to create with the following command:
	// zombiefier -n 10 -> this creates 10 zombies

	
	// create child process
	pid_t childs[number_of_childs];

	
	int exit_code = 0;
	
	int count = 0;
	
	for (int i = 0; i < number_of_childs; i++) {
		childs[i] = fork();

		if (childs[i] < 0) {
		// error occurred
			fprintf(stderr, "Fork Failed"); 
			return 1;
		}

		if (childs[i] == 0) {
			
			//printf("\nchildpid = %d\n", getpid());
			
			exit(1);
		} 
	// at these point we have 'n' zombies
	}

	/* To terminate all zombies, I need to call waitpid on the parent process */
	int i = 0;
	if (childs[i] > 0) {
		int status = pause();
			if (status = -1) {
				//printf("Childs 'reaped'\n");
				waitpid(getpid(), &exit_code, 0);
			}

	}
	return 0;
}