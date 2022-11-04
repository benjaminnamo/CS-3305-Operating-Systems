/*
 *
 * Benjamin Namo
 * 251182806
 * CS3305 Assignment 1
 * 09 23 2022
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

    pid_t i,j,pid;									        //declaring pid type variables
    char pidStr[100];									    //declaring string to hold value we send to external program

    i = getpid();
    printf("parent (PID %d): process started\n",i);
    printf("parent (PID %d): forking child_1\n",i);
    pid = fork();									        //forking parent to create child_1

    if (pid < 0){									        //if pid < 0, creating of child_1 was unsuccesfful
        printf("parent (PID %d): fork unsuccessful for child_1",i);
    }

    if (pid > 0){									        //if pid > 0, creating of child_1 was succesfull and we are in parent
        printf("parent (PID %d): fork successful for child_1 (PID %d)\n",i,pid);
        printf("parent (PID %d): waiting for child_1 (PID %d) to complete\n",i,pid);
        wait(NULL);									        //waiting for child_1 to complete

        printf("parent (PID %d): forking child_2\n",i);	    //after child_1 is completed we fork and create child_2
        pid = fork();									    //forking
        i = getpid();
        j = getppid();


        if (pid < 0){									    //if pid <0, we were unsuccessful in creating child_2
                printf("parent (PID %d): fork unsuccessful",i);
        }

        if (pid > 0){									    //if pid > 0, fork was succesfull and we are in parent
                printf("parent (PID %d): fork successful for child_2 (PID %d)\n",i,pid);
                printf("parent (PID %d): waiting for child_2 (PID %d) to complete\n",i,pid);
                printf("child_2 (PID %d): calling an external program [external_program.out]\n",pid);
                wait(NULL);							    	//waiting for child_2 to complete execution
                printf("parent (PID %d): completed\n",i);
        }

        if (pid == 0){								    	//if pid=0, we are in child_2
                pid = getpid();							    //retrieving pid
                sprintf(pidStr,"%d for child_2",pid);       //adding pid string to pidStr variable
                const char * pidPtr = pidStr;			    //creating cons char pointer that points to pid string
                execl("external_program.out",pidPtr,NULL);	//calling external pgoram that takes over, gives output, then terminates
      }
    }

    if (pid == 0){									        //if the pid is equal to 0, we are in the child process, child_1
      i = getpid();    
      j = getppid();    

      printf("child_1 (PID %d): forking child_1.1\n",i);
        pid = fork();								    	//forking to create child_1.1

            if (pid < 0){								    //if pid <0, fork was unsuccesful
                printf("child_1 (PID %d): fork unsuccessful",i);
            }

            if (pid > 0){								    //if pid >0, fork was succesful and we are in parent(child_1)

                printf("child_1 (PID %d): fork success for child_1.1 (PID %d)\n",i,pid);
                printf("child_1 (PID %d): waiting for child_1.1 (PID %d) to complete\n",i,pid);
                printf("child_1.1 (PID %d): calling an external program [external_program.out]\n",pid);
                wait(NULL);								    //waiting for child_1.1 to complete execution
                printf("child_1 (PID %d): completed\n",i);

            }

            if (pid == 0){								    //if pid =0, we are in child_1.1
                pid = getpid();								//retrieving pid
                sprintf(pidStr,"%d for child_1.1",pid);		//adding pid string to pidStr variable
                const char * pidPtr = pidStr;				//creating const char pointer that points to pid string
                execl("external_program.out",pidPtr,NULL);	//calling external program that takes over, gives output, then terminates

            }
        }

    }
