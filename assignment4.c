#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 100		                                 //maximum lines to read from file
#define MAX_LEN 1000		                                 //maximum length per line

struct process
{				                                             //struct to hold process information
    char *name;
    int arrivalTime;
    int burstTime;
    int timeElapsed;
    int totalTimeElapsed;
    int turnAround;
    int waitTime;
    bool isReady;
};

int main ()
{
    float totalTurnAround = 0;
    float totalWaitTime = 0;
    char lines[MAX_LINES][MAX_LEN];	                         //2d char array that will hold the lines read from file
    FILE *fp;			                                     //file pointer
    fp = fopen ("rr_input.txt", "r");	         //opening file

    if (fp == NULL)
    {				                                         //error message if error in file opening
        perror ("Unable to open data file.");
        exit (1);
    }

    int line = 0;			                                 //variable to keep track of how many lines have been read

    while (!feof (fp) && !ferror (fp))
    {				                                         //while we haven't reached end of file and no errors
        if (fgets (lines[line], MAX_LEN, fp) != NULL)
        {			                                         //if fgets doesn't return null (end of file), continue
            line++;		//increment to next line
        }

    }

    for (int i = 0; i < line; i++)
    {				                                        //iterating through all test cases/inputs

        printf ("Test case #%d: %s", i + 1, lines[i]);
        int lineLen = strlen (lines[i]);	                //length of current case string
        int numProcess = (lineLen - 1) / 7;	                //number of processes = (length-1)/7
        int quantum = (lines[i][lineLen - 2]) - '0';	    //quantum is the value at last index
        printf ("Number of processes: %d, Quantum: %d\n", numProcess,
                quantum);
        char *p = strtok (lines[i], " ");	        //pointer for token splitting
        char *stringSplit[lineLen];	                        //array to store tokens, all the parts of our string

        int count = 0;		                                //keeping track of how many tokens
        int j = 0;

        while (p != NULL)		                            //while we haven't reached end of line
        {
            stringSplit[j++] = p;	                        //add current token to array
            p = strtok (NULL, " ");	            //find next token
            count += 1;		                                //keep track of tokens
        }

        char *processNames[numProcess + 1];	                //process names

        int k = 0;
        while (k < count - 1)
        {
            if (k % 3 == 0)
            {
                processNames[k / 3] = stringSplit[k];
            }
            k++;
        }

        int arrivalTimes[numProcess];	                    //arrival times

        k = 0;
        while (k < count - 1)
        {
            if (k % 3 == 0)
            {
                arrivalTimes[k / 3] = atoi (stringSplit[k + 1]);
            }
            k++;
        }


        int burstTimes[numProcess];	                        //burst times
        k = 0;
        while (k < count - 1)
        {
            if (k % 3 == 0)
            {
                burstTimes[k / 3] = atoi (stringSplit[k + 2]);
            }
            k++;
        }

        struct process processes[numProcess];

        for (k = 0; k < numProcess; k++)
        {
            processes[k].name = processNames[k];
            processes[k].arrivalTime = arrivalTimes[k];
            processes[k].burstTime = burstTimes[k];
            processes[k].timeElapsed = 0;
            processes[k].isReady = false;
            processes[k].totalTimeElapsed = 0;
        }

        int maxCpuTime = 0;

        for (k = 0; k < numProcess; k++)
        {			                                        //total CPU time = total burst + p1 arrival
            maxCpuTime += processes[k].burstTime;
        }

        maxCpuTime += processes[0].arrivalTime;
        int totalTime = 0;
        bool skip = false;
        char string2arr[maxCpuTime][20];

        while (totalTime != maxCpuTime)
        {

            for (k = 0; k < quantum; k++)
            {			                                     //run first process in array time quantum times
                processes[0].timeElapsed += 1;
                char string2[30];
                int stringIndex = 0;


                if (processes[0].timeElapsed < processes[0].burstTime + 1)
                {

                    for (int n = 0; n < numProcess; n++)
                    {
                        processes[n].totalTimeElapsed += 1;	//need to make arrays of everything to print
                    }

                    if (processes[0].totalTimeElapsed - 1 <
                        processes[0].arrivalTime)
                    {
                        sprintf (string2, "None");
                        stringIndex += processes[0].totalTimeElapsed - 1;
                        strcpy (string2arr[stringIndex], string2);
                        skip = true;
                    }

                    else
                    {
                        if (skip == true)
                        {
                            processes[0].timeElapsed -= 1;
                            k -= 1;
                        }

                        //if process is not ready, find the next process in the queue that is ready

                        if (processes[0].totalTimeElapsed + 1 >
                            processes[0].arrivalTime)
                        {
                            processes[0].isReady = true;
                        }

                        else
                        {
                            processes[0].isReady = false;
                        }

                        //if process is ready, continue

                        if (processes[0].isReady == true)
                        {
                            sprintf (string2, "%s [%d/%d]", processes[0].name,
                                     processes[0].timeElapsed,
                                     processes[0].burstTime);
                            stringIndex += processes[0].totalTimeElapsed - 1;
                            strcpy (string2arr[stringIndex], string2);
                            skip = false;
                        }
                    }
                }
            }

            if (skip == false)
            {
                processes[numProcess] = processes[0];	//move process to end of array
                for (k = 0; k < numProcess; k++)
                {
                    if (processes[0].totalTimeElapsed >=
                        processes[0].arrivalTime + 1)
                    {
                        processes[k] = processes[k + 1];	//move all processes forwards
                    }
                }

                //needed to move process that had just arrived to the end of the queue, unsure how to implement for now.

                totalTime += 1;
            }
        }

        for (k = 0; k < maxCpuTime; k++)                //iterate through max cpu time
        {
            printf ("CPU time %d: ", k);                //for everytime print the corresponding statement
            for (j = 0; j < numProcess; j++)
            {
                if (processes[j].arrivalTime == k)      //check for arriving processes
                {
                    printf ("[%s Arrived] ", processes[j].name);
                }

            }
            printf ("%s\n", string2arr[k]);             //print the string of the process

            char *subString;
            char tempString[20];
            char *val1;
            char *val2;

            if (strlen (string2arr[k]) >= 5)         //i.e it's not none (4)
            {
                subString = strtok (string2arr[k], "[");
                subString = strtok (NULL, "]");                 //here we are parsing the string into vars

                strcpy (tempString, subString);
                val1 = strtok (tempString, "/");
                val2 = strtok (NULL, "");

                int v1 = atoi (val1);
                int v2 = atoi (val2);

                int turnAround;
                int waitTime;

                if (v1 == v2)                                           //if process is completed
                {

                    char stringtoFind[10];

                    for (j = 0; j < numProcess; j++)
                    {
                        strcpy (stringtoFind, processes[j].name);       //parsing process name
                        strcat (stringtoFind, " ");

                        if (strcmp (string2arr[k], stringtoFind) == 0)
                        {
                            processes[j].turnAround =                   //calculating turnaround, wait time, total turn around, total wait
                                    k - processes[j].arrivalTime + 1;
                            processes[j].waitTime =
                                    processes[j].turnAround - processes[j].burstTime;
                            turnAround = processes[j].turnAround;
                            totalTurnAround += processes[j].turnAround;
                            waitTime = processes[j].waitTime;
                            totalWaitTime += processes[j].waitTime;
                        }
                    }

                    printf
                            ("Process %scompleted with Turn Around Time: %d, Waiting Time: %d\n",       //print
                             string2arr[k], turnAround, waitTime);

                }
            }
        }
        printf("Process scheduling completed with Avg Turn Around Time: %.2f, Avg waiting time: %.2f\n",totalTurnAround/numProcess,totalWaitTime/numProcess);
        printf("\n");
    }
    return 0;
}
