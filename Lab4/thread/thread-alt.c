/*
 * thread.c - simple example demonstrating the creation of threads
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>


int Value = 0;

int main() {

    pid_t pid; 
    int status;

    pid = fork();

    if (pid == -1) {
        printf("Error forking.\n");
        return -1;
    }

    if (pid == 0) {  // that means it's the children process
        Value = 1;
    } 
    else {     // father's process
        Value = 2;

        pid = wait(&status);

        Value = 3;
    
        exit(0);
   }   

   printf("Value = %d\n",Value);
   return 0;
}
