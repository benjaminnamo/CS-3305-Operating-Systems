/*
 *
 * Benjamin Namo
 * 251182806
 * CS3305 Assignment 2
 * 10 11 2022
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]){

pid_t pid,i;                                                    //declaring pid_t type variables
char string[100];                                               //string to hold our string
int port[2];                                                    //this pipe will be for writing in child and reading in parent
int port2[2];                                                   //this pipe will be for reading in child and writing in parent
i = getpid();

if (pipe(port) < 0){                                            //checking if pipe failed
    perror("pipe error");
    exit(0);
}

if (pipe(port2) < 0){                                           //checking if pipe failed
        perror("pipe error");
        exit(0);
}

pid = fork();                                                   //forking

if (pid < 0) {                                                  //checking if fork failed
    perror("fork error");
    exit(0);
}
if (pid > 0) {                                                  //parent

    close(port2[0]);                                            //closing read pipe end in port 2, since not used.

    printf("parent (PID %d): created child (PID %d)\n", i, pid);

    if (read(port[0],&string,sizeof(string))==-1){              //parent process will wait for child to write in port
        printf("Error in reading from pipe.");                  //error handling
        exit(0);
    }

    printf("parent (PID %d): read from pipe \"%s\"\n", i, string);
    printf("parent (PID %d): received Y = \"%s\"\n",i,argv[2]);
    printf("parent (PID %d): \"%s\" + Y = \"%s %s\"\n",i,string,string,argv[2]);
    strcat(string," ");                                         //adding space to maintain spacing in string sentence
    strcat(string,argv[2]);                                     //concatenating string
    if (write(port2[1],string,sizeof(string))==-1){             //writing string into pipe
        printf("Error in writing to pipe.");                    //error handling
        exit(0);
    }
    printf("parent (PID %d): writing into pipe \"%s\"\n", i, string);

    wait(NULL);                                                 //pausing process in order to go to child

    if (read(port[0],&string,sizeof(string))==-1){              //once child is completed, read from pipe
        printf("Error in reading from pipe.");                  //error handling
        exit(0);
    }
    printf("parent (PID %d): read from pipe \"%s\"\n", i, string);
    printf("parent (PID %d): all tasks completed\n", i);



}

if (pid == 0) {
    pid = getpid();
    close(port[0]);                                             //closing read pipe end in port since not used

    printf("child (PID %d): received X = \"%s\"\n", pid, argv[1]);
    printf("child (PID %d): writing \"%s\" into pipe\n", pid, argv[1]);

    if (write(port[1],argv[1],sizeof(argv[1]))==-1){            //program switches back to parent process since data is now present
        printf("Error in writing to pipe.");                    //error handling
        exit(0);
    }

    if (read(port2[0],&string,sizeof(string))==-1){             //reading from pipe
        printf("Error in reading from pipe.");                  //error handling
        exit(0);
    }
    printf("child (PID %d): read from pipe \"%s\"\n", pid, string);
    printf("child (PID %d): received Z = \"%s %s\"\n", pid, argv[3],argv[4]);
    printf("child (PID %d): \"%s\" + Z = \"%s %s %s\"\n",pid,string,string,argv[3],argv[4]);
    strcat(string," ");                                         //adding spaces and concatenating
    strcat(string,argv[3]);
    strcat(string," ");
    strcat(string,argv[4]);
    printf("child (PID %d): writing into pipe \"%s\"\n", pid, string);
    if (write(port[1],string,sizeof(string))==-1) {             //writing to pipe
        printf("Error in writing to pipe.");                    //error handling
        exit(0);
    }
    printf("child (PID %d): all tasks completed\n", pid);
}
}


