#ifndef AUEBSHELL3_H
#define AUEBSHELL3_H

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
   printf("auebsh3> ");
   line = ReadLine();
   args = SplitLine(line);
   auebsh3_exec(args);
   free(line);
   free(args);
} 

}//end main


void auebsh3_exec(char **args){

int* count;

count = PipeNumber(args);

if(*count == 0){
  auebsh2_exec(args);
}
else{

   pid_t child[2];
   int pipes[2];
   char ** args1;
   int check;

   check = pipe(pipes);

   if(check == -1){
      printf("Error: Pipe failed!\n");
      return;
   }

   for(int i=0; i<2; i++){

      child[i] = fork();

      if(child[i] < 0){
         perror("Error: Fork failed!\n");
	 return;
      }
      if(child[i]==0){ //Child
         if(i==0){ 
	    dup2(pipes[1],1);
         }
         if(i==1){
	    dup2(pipes[0],0);
         }

         close(pipes[0]);
	 close(pipes[1]);

         args1 = SplitCommand(args,i+1);

         int check2;

         check2 = CheckInOut(args);

         if(check2 == 1){
            int fd[2];
            int check3;
            char ** inout = malloc(2 * sizeof(char*));

            check3 = pipe(fd);

            if(check3 == -1){
               printf("Error: Pipe failed!\n");
               return;
            }
      
            inout = TakeInOut(args1);
            args1 = TakeArguments(args1);
  
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
            if(execvp(args1[0], args1) == -1) { //command execution.
	       perror("Error: Child couldnt execute command!\n");
	       raise(SIGKILL);
            }
         }
         else{
            if(execvp(args1[0], args1) == -1) { //command execution.
               perror("Error: Child couldnt execute command!\n");
	       raise(SIGKILL);
            }
         }  
      }
   }
   //Father 
   close(pipes[0]);
   close(pipes[1]);

   for(int i=0; i<2; i++){
      waitpid(child[i],NULL,0);
      printf("This is Child with ID: %d!\n", child[i]);
   }
}
free(count);

}// end auebsh3_exec

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

#endif //AUEBSHELL3_H
