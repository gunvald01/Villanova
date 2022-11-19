/*
SJF, FCFS and Priority scheduling simulation.
*/

#include <stdio.h>

#define MAXNUMPROCS 25 //max number of processes
#define FALSE 0
#define TRUE 1

//process data block
typedef struct process
{
	int arrTime; //arrive time
	int burstTime;
	int priority;
	int finishTime;
	int turnAroundTime;
	int waitTime;
	int processed; //flag indicating process scheduling status
	int procNum; //process number indicating arrival sequence
}ProcessInfo;

//function primitives
void getInfo(); //function that reads in data from standard input
void SJF();     //smilates SJF sheduling algorithm
void FCFS();    //simulates FCFS sheduling algorithm
void Priority(); //simulates Priority sheduling algorithm

//global data declaraion
int numProcesses;
float avgSJFWaitTime; //average SJF sheduling algorithm wait time
float avgFCFSWaitTime; //average FCFS sheduling algorithm wait time
float avgPriorityWaitTime; //average Priority sheduling algorithm wait time
ProcessInfo processes[MAXNUMPROCS]; //array containing processes

//topmost function
main()
{
	//Read in process info from standard input
	getInfo(); //get input data
	SJF();     //call SJF simulation
	FCFS();    //call FCFS simulation
	Priority(); //call Priority simulation
	//generate final output
  printf("The average wait time is\n\n");
	printf("SJF: %10.2f milliseconds\n", avgSJFWaitTime);
	printf("FCFS: %9.2f milliseconds\n", avgFCFSWaitTime);
	printf("Priority: %5.2f milliseconds\n", avgPriorityWaitTime);
	return 0;
} 		

void getInfo()
{
	int i;
	int temp;
	scanf("%d", &numProcesses); //read data from standard input
	for (i=0; i<numProcesses; i++)
	{
		scanf("%d", &temp);
		processes[i].arrTime=temp;
		
		scanf("%d", &temp);
		processes[i].burstTime=temp;

		scanf("%d", &temp);
		processes[i].priority=temp;

		processes[i].finishTime=0; //to be calculated for each simulation
		processes[i].turnAroundTime=0; //to be calculated for each simulation
		processes[i].waitTime=0; //to be calculated for each simulation
		processes[i].processed=FALSE; //indicates that process is unscheduled
		processes[i].procNum=i+1; //process ID
	}
}

//Non-preemptive Shortest Job First algorithm.
//Sort from shortest burst time to highest.
void SJF()
{
	int i,j,c,g;
	int arriveTime;
	int finTime;
	float wTime;
	int found;
	int saved_i;
	
	ProcessInfo temp;
	ProcessInfo temp_i; //temporary data holder
	ProcessInfo temp_j; //temporary data holder
	ProcessInfo sjfProcs[MAXNUMPROCS];
	temp_j.arrTime=9999999;
	temp_j.burstTime=999999;
	temp_j.priority=9999999;
	saved_i=0;
	temp_i.arrTime=9999999;
	temp_i.burstTime=9999999;
	temp_i.priority=9999999;
	//make a local working copy of global processes array
	for (i=0; i<numProcesses; i++)
	{
			sjfProcs[i]=processes[i];
	}

	arriveTime=0; //initialize arrive time
	finTime=0;    //initialize finish time
	//m=0;
	//sort input processes from shortest burst time to longest (ascending order)
	for (i=0; i<numProcesses; i++)
	{
		for (j=i; j<numProcesses-1; j++)
		{
			if (sjfProcs[i].burstTime>sjfProcs[j+1].burstTime)
			{
				temp=sjfProcs[i];
				sjfProcs[i]=sjfProcs[j+1];
				sjfProcs[j+1]=temp;
			}
		}
	}

/*
Find processes of equal burst time and perform two different sorts.
1. Those processes that have same arrive time are sorted according to their arrival
in the queue. Meaning that if two or more processes have the same burst time,
and arrive time but one came in before the other (indicated by lower procNum) ensure 
that the lower procNum process precedes the higher procNum process.
Because one could have processes with same arrival time and burst time but arriving in different sequence.
2.Processes with equal burst time but unequal arrive time are sorted according to the arrive time 
in ascending order (they are actually swapped). Let P1[1] at=2 and P3[4] at=0. Swap them makeing 
P3[1] at=0 and P1[4] at=2.
*/	
	for (i=0; i<numProcesses; i++)
	{
		for (j=i; j<numProcesses-1; j++)
		{
			if (sjfProcs[i].burstTime==sjfProcs[j+1].burstTime)
			{
				//sorting processes with same arrive time
				if (sjfProcs[i].arrTime==sjfProcs[j+1].arrTime)
				{
					//checking processes IDs. Swap the higher with lower
					if (sjfProcs[i].procNum>sjfProcs[j+1].procNum)
					{
						temp=sjfProcs[i];
						sjfProcs[i]=sjfProcs[j+1];
						sjfProcs[j+1]=temp;
					}
				}
				//sort processes according to arrive time
				else if (sjfProcs[i].arrTime>sjfProcs[j+1].arrTime)
				{
					temp=sjfProcs[i];
					sjfProcs[i]=sjfProcs[j+1];
					sjfProcs[j+1]=temp;
				}
			}
		}
	}
	
/*
Check if first process arrival time is greater than 0.
If yes, then use the arrival time to initialize the Gantt chart
by setting the finish time=arrival time. Sort of like delay start
*/
	for (i=0; i<numProcesses; i++)
	{
		if ((sjfProcs[i].procNum==1)&&(sjfProcs[i].arrTime>0))
			{
				finTime=sjfProcs[i].arrTime;
			}
	}

/*
Main loop that builds the Gantt chart. When this look starts, all processes
are sorted in ascending order of burst time, processes of equal burst time are sorted
according to the arrival time and arrival sequence in the queue.
First check if process arrive time is less than finish time. If yes, then use it to 
build the Gantt chart. If not find one whose arrive time is less then finish time. Also make
sure that the process has the lowest burst time. Could have processes that arrive sooner bit with 
longer burst time
*/

	found=FALSE; //Flag that indicates if desired process was found.
	for (i=0; i<numProcesses; i++)
	{
		if (sjfProcs[i].arrTime<=finTime)
		{
			finTime=finTime+sjfProcs[i].burstTime;
			sjfProcs[i].processed=TRUE;
			sjfProcs[i].finishTime=finTime;
			sjfProcs[i].turnAroundTime=sjfProcs[i].finishTime-sjfProcs[i].arrTime;
			sjfProcs[i].waitTime=sjfProcs[i].turnAroundTime-sjfProcs[i].burstTime;
		}
		else //sjfProcs[i].arrTime>finTime) process arrive time is greater than finish time
		{
			//initialize temporary variables
			temp_j.arrTime=9999999;
			temp_j.burstTime=999999;
			temp_j.priority=9999999;

			temp_i.arrTime=9999999;
			temp_i.burstTime=9999999;
			temp_i.priority=9999999;
/*
Find a process whose arrive time is less than finish time, has shortest burst time
and arrived the soonest in the queue. Set found flag when process is found.
*/

			for (j=i; j<numProcesses; j++)
			{
				//find Process whose arrival time <= finTime and process it
				
				if ((sjfProcs[j].arrTime<=finTime)&&(sjfProcs[i].processed==FALSE))
				{
					if ((sjfProcs[j].arrTime<temp_j.arrTime)&&(sjfProcs[j].burstTime<temp_j.burstTime))
					{
						found=TRUE;
						temp_i=sjfProcs[i];
						temp_j=sjfProcs[j];
						saved_i=i;
						temp=sjfProcs[i];
						sjfProcs[i]=sjfProcs[j];
						sjfProcs[j]=temp;
					}
				}

//Next two sorts are needed to allow the algorithm to build tha Gantt chart.
//These sorts are identical to sorts performed at the entry into the function.
//Sometimes swapping processes re-arranges the array and sorting the remaining processes
//must be done.
//Resort remaining processes in burst time in ascending order.
//But start the sort with next index that was processed above.
//Reorders remaining processes to help in 
			
				for (g=saved_i+1; g<numProcesses; g++)
				{
					for (c=g; c<numProcesses-1; c++)
					{
						if (sjfProcs[g].burstTime>sjfProcs[c+1].burstTime)
						{
							temp=sjfProcs[g];
							sjfProcs[g]=sjfProcs[c+1];
							sjfProcs[c+1]=temp;
						}
					}
				}
//Sort equal burst time processes in ascending arrive sequence and
//ascending arrive time. Equal arrive time processes are sorted in ascending
//arrive sequence in the queue.
				for (g=saved_i+1; g<numProcesses; g++)
				{
					for (c=g; c<numProcesses-1; c++)
					{
						if (sjfProcs[g].burstTime==sjfProcs[c+1].burstTime)
						{
							if (sjfProcs[g].arrTime==sjfProcs[c+1].arrTime)
							{
								if (sjfProcs[g].procNum>sjfProcs[c+1].procNum)
								{
									temp=sjfProcs[g];
									sjfProcs[g]=sjfProcs[c+1];
									sjfProcs[c+1]=temp;
								}
							}
							else if (sjfProcs[g].arrTime>sjfProcs[c+1].arrTime)
							{
								temp=sjfProcs[g];
								sjfProcs[g]=sjfProcs[c+1];
								sjfProcs[c+1]=temp;
							}
						}//end if (sjfProcs[g].burstTime==sjfProcs[c+1].burstTime)
					} //for (c=g; c<numProcesses-1; c++)
				}//end for (g=saved_i+1; g<numProcesses; g++)
			}//for (j=i; j<numProcesses; j++)

			//Building the Gantt chart
			if (found)
			{
				finTime=finTime+sjfProcs[saved_i].burstTime;
				sjfProcs[saved_i].processed=TRUE;
				sjfProcs[saved_i].finishTime=finTime;
				sjfProcs[saved_i].turnAroundTime=sjfProcs[saved_i].finishTime-sjfProcs[saved_i].arrTime;
				sjfProcs[saved_i].waitTime=sjfProcs[saved_i].turnAroundTime-sjfProcs[saved_i].burstTime;
				found=FALSE;
			}

			//Schedule processes whose arrive time is greater than finish time, arrived in the middle
			//and that were not scheduled by above code. 
			if ((sjfProcs[i].arrTime>finTime)&&(sjfProcs[i].processed==FALSE))
			{
				if ((!found)&&(sjfProcs[i].procNum!=numProcesses))
				{
					finTime=sjfProcs[i].arrTime;
					finTime=finTime+sjfProcs[i].burstTime;
					sjfProcs[i].processed=TRUE;
					sjfProcs[i].finishTime=finTime;
					sjfProcs[i].turnAroundTime=sjfProcs[i].finishTime-sjfProcs[i].arrTime;
					sjfProcs[i].waitTime=sjfProcs[i].turnAroundTime-sjfProcs[i].burstTime;
				}
			}
			
			//Schedule last process whose arrive time is greater than finish.
			//This requires adding a delay to last process.
			if ((sjfProcs[i].procNum==numProcesses)&&(sjfProcs[i].processed==FALSE))
			{
				finTime=sjfProcs[i].arrTime;
				finTime=finTime+sjfProcs[i].burstTime;
				sjfProcs[i].processed=TRUE;
				sjfProcs[i].finishTime=finTime;
				sjfProcs[i].turnAroundTime=sjfProcs[i].finishTime-sjfProcs[i].arrTime;
				sjfProcs[i].waitTime=sjfProcs[i].turnAroundTime-sjfProcs[i].burstTime;
			}
		}//else //sjfProcs[i].arrTime>finTime)
	}//end for (i=0; i<numProcesses; i++)
	
	//calculate waiting time
	avgSJFWaitTime=0.0f;
	wTime=0.0f;
	for (i=0; i<numProcesses; i++)
	{
		wTime=sjfProcs[i].waitTime+wTime;
	}
	avgSJFWaitTime=wTime/numProcesses;
}

//First Come First Serve scheduling simulation
void FCFS()
{
	int i;
	int arriveTime=0;
	int finTime=0;
	float wTime;
	ProcessInfo fcfsProcs[MAXNUMPROCS];

	avgFCFSWaitTime=0.0f;

	//make a local working copy of global processes array
	for (i=0; i<numProcesses; i++)
	{
		fcfsProcs[i]=processes[i];
	}

	//Build non-preemptive FCFS Gantt chart for each Process.
	for (i=0; i<numProcesses; i++)
	{
		//for first process only. Initialize Gantt chart
		if (i==0) 
		{
			if (fcfsProcs[i].arrTime==0)
			{
				arriveTime=fcfsProcs[i].arrTime;
				finTime=fcfsProcs[i].burstTime;
			}
			else
			{
				finTime=fcfsProcs[i].burstTime+fcfsProcs[i].arrTime;
			}
			
		}
		else //for i>0
		{
			//check if arrival time is greater than last finish time.
			//This means that CPU has been idle and must change
			//finishTime to current process arrivalTime to keep
			//track of CPU processing time.
			if (fcfsProcs[i].arrTime > finTime) 
			{
				finTime=fcfsProcs[i].arrTime;
				finTime=finTime+fcfsProcs[i].burstTime;
			}
			else //process arrival time is 
				  //less than prior process finish time.
			{
				finTime=finTime+fcfsProcs[i].burstTime;
			}
		}
		fcfsProcs[i].finishTime=finTime;
		fcfsProcs[i].turnAroundTime=fcfsProcs[i].finishTime-fcfsProcs[i].arrTime;
		fcfsProcs[i].waitTime=fcfsProcs[i].turnAroundTime-fcfsProcs[i].burstTime;
	}

	avgFCFSWaitTime=0.0f;
	wTime=0.0f;
	for (i=0; i<numProcesses; i++)
	{
		wTime=fcfsProcs[i].waitTime+wTime;
	}
	avgFCFSWaitTime=wTime/numProcesses;
}

/*
Priority based scheduling simulation. Basically the same algorithm as SJF simulation.
Except use descending priority order. order 
*/
void Priority()
{
	int i,j,c,g;
	int arriveTime;
	int finTime;
	float wTime;
	int found;
	int saved_i;
	
	ProcessInfo temp;
	ProcessInfo temp_i;
	ProcessInfo temp_j;
	ProcessInfo priProcs[MAXNUMPROCS];
	temp_j.arrTime=9999999;
	temp_j.burstTime=999999;
	temp_j.priority=0;
	saved_i=0;
	temp_i.arrTime=9999999;
	temp_i.burstTime=9999999;
	temp_i.priority=0;
	//make a local working copy of global processes array
	for (i=0; i<numProcesses; i++)
	{
			priProcs[i]=processes[i];
	}

	//Sort processes in descending order in priority
	//Use that array to build the Gantt chart.
	arriveTime=0;
	finTime=0;
	//m=0;
	//sort highest priority
	for (i=0; i<numProcesses; i++)
	{
		for (j=i; j<numProcesses-1; j++)
		{
			if (priProcs[i].priority<priProcs[j+1].priority)
			{
				temp=priProcs[i];
				priProcs[i]=priProcs[j+1];
				priProcs[j+1]=temp;
			}
		}
	}
/*
Find processes of equal priority and perform two different sorts.
1. Those processes that have same arrive time are sorted according to their arrival
in the queue. Meaning that if two or more processes have the same burst time,
and arrive time but one came in before the other (indicated by lower procNum) ensure 
that the lower procNum process precedes the higher procNum process.
Because one could have processes with same arrival time and priority but arriving in different sequence.
2.Processes with equal priority but unequal arrive time are sorted according to the arrive time 
in ascending order (they are actually swapped). Let P1[1] at=2 and P3[4] at=0. Swap them makeing 
P3[1] at=0 and P1[4] at=2.
*/	
	for (i=0; i<numProcesses; i++)
	{
		for (j=i; j<numProcesses-1; j++)
		{
			if (priProcs[i].priority==priProcs[j+1].priority)
			{
				if (priProcs[i].arrTime==priProcs[j+1].arrTime)
				{
					if (priProcs[i].procNum>priProcs[j+1].procNum)
					{
						temp=priProcs[i];
						priProcs[i]=priProcs[j+1];
						priProcs[j+1]=temp;
					}
				}
				else if (priProcs[i].arrTime>priProcs[j+1].arrTime)
				{
					temp=priProcs[i];
					priProcs[i]=priProcs[j+1];
					priProcs[j+1]=temp;
				}
			}
		}
	}

//check if first process arrival time is greater than 0.
//If yes, then use the arrival time to initialize the Gantt chart
//by setting the finish time=arrival time. Sort of like delay start
	for (i=0; i<numProcesses; i++)
	{
		if ((priProcs[i].procNum==1)&&(priProcs[i].arrTime>0))
			{
				finTime=priProcs[i].arrTime;
			}
	}
/*
Main loop that builds the Gantt chart. When this look starts, all processes
are sorted in descending order in priority, processes of equal burst time are sorted
according to the arrival time and arrival sequence in the queue.
First check if process arrive time is less than finish time. If yes, then use it to 
build the Gantt chart. If not find one whose arrive time is less then finish time. Also make
sure that the process has the hihest priority. Could have processes that arrive sooner bit with 
lower priority.
*/
	found=FALSE;
	for (i=0; i<numProcesses; i++)
	{
		if (priProcs[i].arrTime<=finTime)
		{
			finTime=finTime+priProcs[i].burstTime;
			priProcs[i].processed=TRUE;
			priProcs[i].finishTime=finTime;
			priProcs[i].turnAroundTime=priProcs[i].finishTime-priProcs[i].arrTime;
			priProcs[i].waitTime=priProcs[i].turnAroundTime-priProcs[i].burstTime;
		}
		else //priProcs[i].arrTime>finTime)
		{
				
			temp_j.arrTime=9999999;
			temp_j.burstTime=999999;
			temp_j.priority=0;

			temp_i.arrTime=9999999;
			temp_i.burstTime=9999999;
			temp_i.priority=0;
/*
Find a process whose arrive time is less than finish time, has the highest priority
and arrived the soonest in the queue. Set found flag when process is found.
*/
			for (j=i; j<numProcesses; j++)
			{
				if ((priProcs[j].arrTime<=finTime)&&(priProcs[i].processed==FALSE))
				{
					if ((priProcs[j].arrTime<temp_j.arrTime)&&(priProcs[j].priority>temp_j.priority))
					{
						found=TRUE;
						temp_i=priProcs[i];
						temp_j=priProcs[j];
						saved_i=i;
						temp=priProcs[i];
						priProcs[i]=priProcs[j];
						priProcs[j]=temp;
					}
				}
//Next two sorts are needed to allow the algorithm to build tha Gantt chart.
//These sorts are identical to sorts performed at the entry into the function.
//Sometimes swapping processes re-arranges the array and sorting the remaining processes
//must be done.
//Resort remaining processes in priority descending order.
//But start the sort with next index that was processed above.			
				for (g=saved_i+1; g<numProcesses; g++)
				{
					for (c=g; c<numProcesses-1; c++)
					{
						if (priProcs[g].priority<priProcs[c+1].priority)
						{
							temp=priProcs[g];
							priProcs[g]=priProcs[c+1];
							priProcs[c+1]=temp;
						}
					}
				}
//Sort equal priority processes in ascending arrive sequence and
//ascending arrive time. Equal arrive time processes are sorted in ascending
//arrive sequence in the queue.
				for (g=saved_i+1; g<numProcesses; g++)
				{
					for (c=g; c<numProcesses-1; c++)
					{
						if (priProcs[g].priority==priProcs[c+1].priority)
						{
							if (priProcs[g].arrTime==priProcs[c+1].arrTime)
							{
								if (priProcs[g].procNum>priProcs[c+1].procNum)
								{
									temp=priProcs[g];
									priProcs[g]=priProcs[c+1];
									priProcs[c+1]=temp;
								}
							}
							else if (priProcs[g].arrTime>priProcs[c+1].arrTime)
							{
								temp=priProcs[g];
								priProcs[g]=priProcs[c+1];
								priProcs[c+1]=temp;
							}	
						}//end if (priProcs[g].priority==priProcs[c+1].priority)
					}//end for (c=g; c<numProcesses-1; c++)
				}//end for (g=saved_i+1; g<numProcesses; g++)
			}//end for (j=i; j<numProcesses; j++)

//Schedule process was found above.
			if (found)
			{
				finTime=finTime+priProcs[saved_i].burstTime;
				priProcs[saved_i].processed=TRUE;
				priProcs[saved_i].finishTime=finTime;
				priProcs[saved_i].turnAroundTime=priProcs[saved_i].finishTime-priProcs[saved_i].arrTime;
				priProcs[saved_i].waitTime=priProcs[saved_i].turnAroundTime-priProcs[saved_i].burstTime;
				found=FALSE;
			}

			//Schedule processes whose arrive time is greater than finish time, arrived in the middle
			//and that were not scheduled by above code. 
			if ((priProcs[i].arrTime>finTime)&&(priProcs[i].processed==FALSE))
			{
				if ((!found)&&(priProcs[i].procNum!=numProcesses))
				{
					finTime=priProcs[i].arrTime;
					finTime=finTime+priProcs[i].burstTime;
					priProcs[i].processed=TRUE;
					priProcs[i].finishTime=finTime;
					priProcs[i].turnAroundTime=priProcs[i].finishTime-priProcs[i].arrTime;
					priProcs[i].waitTime=priProcs[i].turnAroundTime-priProcs[i].burstTime;
				}
			}
			//Schedule last process whose arrive time is greater than finish.
			//This requires adding a delay to last process.
			if ((priProcs[i].procNum==numProcesses)&&(priProcs[i].processed==FALSE))
			{
				finTime=priProcs[i].arrTime;
				finTime=finTime+priProcs[i].burstTime;
				priProcs[i].processed=TRUE;
				priProcs[i].finishTime=finTime;
				priProcs[i].turnAroundTime=priProcs[i].finishTime-priProcs[i].arrTime;
				priProcs[i].waitTime=priProcs[i].turnAroundTime-priProcs[i].burstTime;
			}
		}//end else //priProcs[i].arrTime>finTime)
	}// end for (i=0; i<numProcesses; i++)

	avgPriorityWaitTime=0.0f;
	wTime=0.0f;
	for (i=0; i<numProcesses; i++)
	{
		wTime=priProcs[i].waitTime+wTime;
	}
	avgPriorityWaitTime=wTime/numProcesses;
}