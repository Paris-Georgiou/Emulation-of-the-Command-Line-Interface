#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include "auebsh.h"

#define DELIM " \t\r\n\a"

char* ReadLine(){

int position = 0;
char* buffer = malloc(255 * sizeof(char));
int c;

if(!buffer){
   perror("Buffer: Allocation Error!\n");
   exit(EXIT_FAILURE);
}

while(1){

   c = getchar(); // Read a character

   if(c == EOF){ // If we hit EOF, exit shell.
      printf("Closing Program!\n");
      exit(1);
   }
   else if(c == '\n'){
      buffer[position] = '\0';
      return buffer;
   }
   else{
      buffer[position] = c;
   } 
   position++;

   if(position >= 255){ // If we have exceeded the buffer, exit.
      perror("Command Error: Too big to execute!\n");
      exit(EXIT_FAILURE);
   }
 }

}//end ReadLine

char** SplitLine(char *line){

int position = 0;
char** args = malloc(255 * sizeof(char*));

if(!args){
   perror("Tokens: Allocation Error!\n");
   exit(EXIT_FAILURE);
}

args[position] = strtok(line, DELIM);
while (args[position] != NULL) {  
   position++;
   args[position] = strtok(NULL, DELIM);
}
  return args;
}//end SplitLine

int CheckInOut(char **args){

int position = 0;
int check = 0;

while(args[position] != NULL){
   if(strcmp(args[position],"<") == 0){
     check = 1;
   }
   if(strcmp(args[position],">") == 0){
     check =  1;
   }
   else if(strcmp(args[position],">>") == 0){
      printf("Syntax Error!\n");
      check = 0;
      return check;
   }
   position++;
}

return check;
}//end CheckInOut

char** TakeArguments(char** args){

int position = 0;
char** args1 = malloc(255 * sizeof(char*));

while(args[position] != NULL){
   if(strcmp(args[position],"<") == 0){
      break;
   }
   if(strcmp(args[position],">") == 0){
      break;
   }
   args1[position] = args[position];
   position++;
}

return args1;
}//end TakeArguments

char **TakeInOut(char **args){

int position = 0;	
char** inout = malloc(2 * sizeof(char*));

while(args[position] != NULL){
   if(strcmp(args[position],"<") == 0){
      position++;
      inout[0]= args[position];
   }
   else if(strcmp(args[position],">") == 0){
      position++;
      inout[1] = args[position];
   }
   else{
      position++;
   }
}

return inout;

}//end TakeInOut

int* PipeNumber(char **args){

int position = 0;
int number = 0;
int* pipes = malloc(sizeof(int));

while(args[position] != NULL){
   if(strcmp(args[position],"|") == 0){
      number++;
   }
   position++;
}

*pipes = number;

return pipes;
  
}//end PipeNumber

char** SplitCommand(char** args, int number){

int position = 0;
int position2 = 0;
int count = 1;
char** args1 = malloc(255 * sizeof(char*));

while(args[position] != NULL){
   if(strcmp(args[position],"|") == 0){
      count++; 
      position++;
      if(count>number){
         break;
      }
   }
   if(number == count){
      args1[position2] = args[position];
      position2++;
   }
   position++;
}

return args1;
}//end of SplitCommand
#endif // COMMON_H
