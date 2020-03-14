#ifndef AUEBSHELL1_H
#define AUEBSHELL1_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "auebsh.h"

void main(){

char *line;
char **args;

while(1){
   printf("auebsh1> ");
   line = ReadLine();
   args = SplitLine(line);
   auebsh1_exec(args); 
   free(line);
   free(args);
}
   
}// end main

void auebsh1_exec(char **args){
	
pid_t child;

child = fork();

if(child < 0){
   perror("Error: Fork failed!\n");
}
else if(child == 0){ //Child
   if(execvp(args[0], args) == -1) { //command execution.
      perror("Error: Child couldnt execute command!\n");
      raise(SIGKILL);
   }
}
else{ //Father
   waitpid(-1,NULL,0); //Father waits for the child to finish the process
}
	
}//end auebsh1_exec

#endif //AUEBSHELL1_H
