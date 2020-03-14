#ifndef AUEBSHELL2_H
#define AUEBSHELL2_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "auebsh.h"

void main(){

char *line;
char **args;

while(1){
   printf("auebsh2> ");
   line = ReadLine();
   args = SplitLine(line);
   auebsh2_exec(args);
   free(line);
   free(args);
}
  
}//end main

void auebsh2_exec(char **args){ 
	
pid_t child;

child = fork();

if(child < 0){
   perror("Error: Fork failed.\n");
}
else if(child == 0){ //Child
   int check;

   check = CheckInOut(args);

   if(check == 1){
      int fd[2];
      int check2;
      char ** inout = malloc(2 * sizeof(char*));

      check2 = pipe(fd);

      if(check2 == -1){
         printf("Error: Pipe failed!\n");
         return;
      }
      
      inout = TakeInOut(args);
      args = TakeArguments(args);
  
      if(inout[0] != NULL){
         fd[0] = open(inout[0], O_RDONLY);
         if(fd[0] < 0){
            perror("Error: Open File failed!\n");
            raise(SIGKILL);
         }
         dup2(fd[0], STDIN_FILENO);
         close(fd[0]);
      }
      if(inout[1] != NULL){
         fd[1] = creat(inout[1], 0644);
	 if(fd[1] < 0){
            perror("Error: Write File failed!\n");
            raise(SIGKILL);
         }
         dup2(fd[1], STDOUT_FILENO);
         close(fd[1]);
      }
      free(inout); 

      if(execvp(args[0], args) == -1) { //command execution.
         perror("Error: Child couldnt execute command!\n");
         raise(SIGKILL);
      }
   }  
   else{
      if(execvp(args[0], args) == -1) { //command execution.
         perror("Error: Child couldnt execute command!\n");
	 raise(SIGKILL);
      }
   }
}
else{ //Father
   waitpid(-1,NULL,0); //Father waits for the child to finish the process    
}
		
}//end auebsh2_exec

#endif //AUEBSHELL2_H
