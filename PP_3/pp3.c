#include <sys/types.h>
#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/wait.h>


#define BUFFER_SIZE 50

/* Rene Figueroa 
   008894574
   CMPE 142
   Programming Project 3 */

int main() {

   
   int exit_code = 0;
   int parent_pipe[2], child_pipe[2];
   int parent_status_pipe, child_status_pipe;

   int pid;

   char readmessage[BUFFER_SIZE];
   parent_status_pipe = pipe(parent_pipe);
   
   if (parent_status_pipe == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   child_status_pipe = pipe(child_pipe);
   
   if (child_status_pipe == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }

   pid = fork();

   
   if (pid != 0) {// Parent process {
      // Close the read side of the parent and write side of child
      close(parent_pipe[0]); 
      close(child_pipe[1]); 

      // use dprintf since it uses file descriptors -> ref man dprintf page
      dprintf(parent_pipe[1], "I am your daddy! and my name is %d\n", getpid());
      read(child_pipe[0], readmessage, sizeof(readmessage));

      fprintf(stdout, "%s",readmessage);
      

      waitpid(getpid(), &exit_code, 0);
   } else { //child process
      // close the write side of parent and read side of child
      close(parent_pipe[1]); 
      close(child_pipe[0]); 

      read(parent_pipe[0], readmessage, sizeof(readmessage));
      printf("%s", readmessage);

      
      dprintf(child_pipe[1], "Daddy, my name is %d", getpid());
      

   }
   return 0;
}


