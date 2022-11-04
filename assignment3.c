#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
void *subtract(void *thread_id);
void *prime_check(void *thread_id);
void *reverse_num(void *thread_id);

int main(int argc, char* argv[]){

    char *token;
    char *search = "/";

    pid_t parentPID;                                                //parent PID variable
    parentPID = getpid();                                           //getting parent ID

    pthread_t threadID;

    pthread_t thread_1,thread_2,thread_3;

    char string[100];
    char tempString[100];

    int int1 = atoi(argv[1]);                                       //converting char arguments to integers
    int int2 = atoi(argv[2]);

    int port[2];                                                    //declare pipe ports

    if (pipe(port) < 0){                                     	    //checking if pipe failed//initializing pipe
        printf("pipe error");
        exit(0);
    }

    if ((int1<0)||(int2<0)){                                        //checking for negative numbers.
        printf("Integer cannot be negative.\n");
        exit(0);
    }

    if (int1<=int2){                                                //checking for valid size of numbers
        printf("The first integer must be larger than the second. \n");
        exit(0);
    }

    void *subtract(void *thread_id){
        int subResult;

        if (read(port[0],&tempString,sizeof(string))==-1){	    //reading string from pipe
		printf("Error in reading code.");
		exit(1);
	}

        token = strtok(tempString,search);                          //taking first integer
        int x = atoi(token);                                        //assigning this to X
        token = strtok(NULL,search);                                //taking second integer
        int y = atoi(token);                                        //assigning this to Y

        printf("thread (TID 1) reads X = %d and Y = %d\n",x,y);
        printf("thread (TID 1) writes X-Y = %d to the pipe\n",x-y);
        subResult = (x-y);

        if (write(port[1],&subResult,sizeof(subResult))==-1){	    //write to pipe
		printf("Error in writing to pipe.");
		exit(1);
   	 }
    }

    void *prime_check(void *thread_id){
       int subResult;
       int prime = 1;
	
       if (read(port[0],&subResult,sizeof(subResult))==-1){	    //reading from pipe
	       printf("Error in reading from pipe.");
	       exit(1);
       }

        printf("thread (TID 2) reads X-Y = %d from the pipe\n",subResult);

        if (subResult == 0 || subResult == 1){			    //if result is 0 or 1, it isnt prime
            prime = 0;
        }

        for (int i = 2; i<=subResult/2;i++){			    //if subresult has any factors other than itself, it isnt prime
            if (subResult % i == 0){
                prime = 0;
                break;
            }
        }

        if (prime == 1){
            printf("thread (TID 2) identified that %d is a prime number\n",subResult);
        }

        else{
            printf("thread (TID 2) identified that %d is not a prime number\n",subResult);
        }

       if (write(port[1],&subResult,sizeof(subResult))==-1){	    //writing to pipe
	       printf("Error in writing to pipe.");
	       exit(1);
       }

        printf("thread (TID 2) writes %d to the pipe\n",subResult);
    }

    void *reverse_num(void *thread_id){
       
	int subResult;
        int remainder;
        int revResult;

       if (read(port[0],&subResult,sizeof(subResult))==-1){	    //reading from pipe
	       printf("Error in reading from pipe.");
	       exit(1);
       }

        printf("thread (TID 3) reads X-Y = %d from the pipe\n",subResult);

        while (subResult !=0){					    //reversing number 
            remainder = subResult % 10;
            revResult = revResult * 10 + remainder;
            subResult /= 10;
        }
        printf("thread (TID 3) reversed number is %d\n",revResult);
    }

    void *start_1(){
        threadID = pthread_self();
        subtract((void*)threadID);
        return NULL;
    }

    void *start_2(){
        threadID = pthread_self();
        prime_check((void*)threadID);
        return NULL;
    }

    void *start_3() {
        threadID = pthread_self();
        reverse_num((void *) threadID);
        return NULL;
    }

    printf("parent (PID %d): receives X = %s and Y = %s from the user\n", parentPID, argv[1],argv[2]);
    printf("parent (PID %d): writes X = %s and Y = %s to the pipe\n", parentPID, argv[1],argv[2]);

    strcat(string,argv[1]);
    strcat(string,"/");
    strcat(string,argv[2]);

   if (write(port[1],string,sizeof(string))==-1){		     //main doing the first write to the pipe
	   printf("Error in writing to pipe.");
	   exit(1);
   }

    if (pthread_create(&thread_1,NULL,start_1,NULL)){		     //creating threads and corresponding pthread_join calls
        printf("Error while creating thread.");
        exit(1);
    }

    pthread_join(thread_1, NULL);

    if (pthread_create(&thread_2,NULL,start_2,NULL)){
        printf("Error while creating thread.");
        exit(1);
    }

    pthread_join(thread_2, NULL);

    if (pthread_create(&thread_3, NULL, start_3, NULL)){
        printf("Error while creating thread.");
        exit(1);
    }

    pthread_join(thread_3, NULL);

    exit(0);
}
