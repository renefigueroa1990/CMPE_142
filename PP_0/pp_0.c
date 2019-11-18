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
Programming Project 0
CMPE 142
*/

void signalhandler(int num){
	/*empty signal handler*/
}


int main()
{
	signal(SIGINT,signalhandler); // 2
	signal(SIGHUP, signalhandler); // 1
	signal(SIGKILL, signalhandler);// 9
	signal(SIGTERM, signalhandler); // 15
	signal(SIGTSTP, signalhandler); // 20
	
   
   // create child process
	pid_t pid;
	int pause_status = 0;
	int exitstatus = 0;



pid = fork();

	if (pid < 0) {
		// error occurred
		fprintf(stderr, "Fork Failed"); 
		return 1;
	}

	if (pid == 0) { /* child process */ 

		printf("\nchildpid = %d\n", getpid());
		pause_status = pause();
		/*after system call pause() returns, then exit with status 5*/
		/*pause() returns -1 when signal is caught */
		
		if (pause_status == -1) {
			//fprintf(stderr, "ERRORMSG: %s\n", strerror(errno));
			exit(5);
		}

	}
	else if (pid > 0) { /* parent process */
		
		int return_pid = waitpid(pid,&exitstatus,0); 
		// waitpid returns pid of child that was terminated
		if (return_pid == int(pid)) {
			printf("childpid = %d, exitstatus = %d\n", int(pid),exitstatus);
		} else if (return_pid == -1) {
			fprintf(stderr, "ERRORMSG%s\n", strerror(errno));
		}
		
	   return 0;
	}

}
