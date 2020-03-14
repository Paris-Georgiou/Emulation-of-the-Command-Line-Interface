# Emulation-of-the-Command-Line-Interface

This project revolves around the use of fork() function in C programming language.

File auebsh.h contains the Headers of the functions we want to use.

File auebsh-common.c contains the code of the common fuctions we will use in our main functions.

File auebsh1.c reads an input from the user, checks the input and executes the request (cant read nor write files) using the fork() command.

File auebsh2.c is an extension of auebsh1.c. Now, the programm is able to read files and write on files.

File auebsh3.c is an extension of auebsh2.c. Now, the programm is able to execute 2 requests from the user with 1 input using a pipe.

File auebsh4.c is an extension of auebsh3.c. Now, the programm is able to execute multiple requests from the user with 1 input using as many pipes as needed.
