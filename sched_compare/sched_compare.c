#include <stdio.h>   // for printf()
#include <stdlib.h>  // for atoi()

#include "procs.h"
#include "procheap.h"

// DO NOT CHANGE THESE TWO CONSTANTS !
#define INTER_ARRIVAL_TIME 3   // mean poisson dist
#define SERVICE_TIME       5   // mean poisson dist


double myvalue(proc_t *proc)
{
  return proc->service_time;
}


void printTable(int numprocs, proc_t *procs, int *finishTime, int *turnAroundTime, int policyChoice) {
  
  int i = 0;
  double trTS, mean = 0;

  if(policyChoice == 0) {
    printf("Process \t\t");
  for(i = 0; i < numprocs; i++) {
    printf("%d\t", i);
  }

  printf("\nArrival Time \t\t");
  for(i = 0; i < numprocs; i++) {
    printf("%.0f\t", procs[i].arrival_time);
  }

  printf("\nService Time(Ts) \t");
  for(i = 0; i < numprocs; i++) {
    printf("%.0f\t", procs[i].service_time);
  }
    printf("\n\t\t\t\t FCFS\t\n");
  } else if(policyChoice == 1) {
    printf("\t\t\t\t SPN\t\n");
  } else if (policyChoice == 2) {
    printf("\t\t\t\t HRRN\t\n");
  }

  printf("Finish time \t\t");
  for(i = 0; i < numprocs; i++) {
    printf("%d\t", finishTime[i]);
  }
  
  printf("\nTurnaround Time(Tr) \t");
  for(i = 0; i < numprocs; i++) {
    mean += turnAroundTime[i];
    printf("%d\t", turnAroundTime[i]);
  }
  
  mean /= numprocs;

  printf("%.2f", mean);

  mean = 0;

  printf("\nTr/Ts \t\t\t");
  for(i = 0; i < numprocs; i++) {
    trTS = turnAroundTime[i] / procs[i].service_time;
    mean += trTS;
    printf("%.2f\t", trTS);
  }

  mean /= numprocs;

  printf("%.2f", mean);

  printf("\n");
}

void fcfs(int numprocs, proc_t *procs)
{

  int i, totalTime = 0, serviceCount = 0, finishTime[numprocs], turnAroundTime[numprocs];
  //double trTS, mean = 0;

  for(i = 0; i < numprocs; i++) {
    finishTime[i] = 0;
    turnAroundTime[i] = 0;
  }

  for(i = 0; i < numprocs; i++) {

    if(totalTime < procs[i].arrival_time) {
      while(totalTime < procs[i].arrival_time) {
        totalTime++;
      }
    }

    while(serviceCount < procs[i].service_time) {
      serviceCount++;
      totalTime++;
    }
    //printf("%d\n", serviceCount);
    finishTime[i] += totalTime;
    turnAroundTime[i] = finishTime[i] - procs[i].arrival_time;
    serviceCount = 0;
  }

  //printf("Total time: %d\n", totalTime);

  printTable(numprocs, procs, finishTime, turnAroundTime, 0);
}

void spn(int numprocs, proc_t *procs)
{
  int i = 0, n = 1, p, totalTime = 0,  
	queue = numprocs, complete[numprocs], avail[numprocs],
	finishTime[numprocs], turnAroundTime[numprocs],
	lowestServiceTime = procs[0].service_time, sTimeIndex = 0;
  //double trTS, mean = 0;

  for(i = 0; i < numprocs; i++) {
    finishTime[i] = 0;
    turnAroundTime[i] = 0;
    complete[i] = 0;
    avail[i] = 0;
  }

  /*
  for(i = 0; i < numprocs; i++) {
    if(procs[i].arrival_time <= totalTime) {
      for(n = i; n < numprocs; n++) {
        if(procs[n].arrival_time > totalTime) {
          break;
        }
        
      }
    }
    totalTime++;
  }
  */

  for(i = 0; i < queue; i++) {

    //Find the highest service time, set it to lowestServiceTime for comparison purposes
    for(n = 0; n < numprocs; n++) {
      if(lowestServiceTime < procs[n].service_time) {
        lowestServiceTime = procs[n].service_time;
        sTimeIndex = n;
      }
    }

    //If there isn't a process ready, wait until there is one ready by incrememnting totalTime
    while(totalTime < procs[i].arrival_time) {
      //printf("i: %d tTime: %d\n", i, totalTime);
      //printf("\nQueue: %d, totalTime: %d\n", i, totalTime);
      totalTime++;
    }
    //Checks for available processes
    for(n = 0; n < numprocs; n++) {
      if(complete[n] == 0) { //not processed yet
        if(procs[n].arrival_time <= totalTime) {
          //printf("n: %d\n", n);
          avail[n]++;
        }
      }
    }
    //For loop tests where the next available and unused process is, stores it. 
    //printf("\nQueue: %d, totalTime: %d, LST: %d\n", i, totalTime, lowestServiceTime);
    for(n = 0; n < numprocs; n++) {
      if(complete[n] == 0 && avail[n] > 0) {
        p = n;
        lowestServiceTime = procs[p].service_time;
        sTimeIndex = p;
        //printf("\n[%d]service_time: %.0f, sTimeIndex: %d\n", p, procs[p].service_time, sTimeIndex);
        //printf("\nlowestServiceTime: %.0f, sTimeIndex: %d\n")
        break;
      }
    }
    
    for(n = p; n < numprocs; n++) {
      //printf("avail[%d]: %d\n", i, avail[i]);
      //printf("%d service time:  %.0f\n", n, procs[n].service_time);
      if(avail[n] == 0) {
	//printf("Position %d is not available yet, break\n", n);
        break;
      }
      if(complete[n] == 0 && procs[n].service_time < lowestServiceTime) {
        //printf("Made it through here %d times\n", n);
        lowestServiceTime = procs[n].service_time;
        sTimeIndex = n;
      }
    }
    
    for(n = 0; n < procs[sTimeIndex].service_time; n++) {
      totalTime++;
    }
    finishTime[sTimeIndex] = totalTime;
    turnAroundTime[sTimeIndex] = finishTime[sTimeIndex] - procs[sTimeIndex].arrival_time;
    complete[sTimeIndex]++;
    //printf("\nFinish time[%d]: %d, turnAroundTime: %d\n\n", sTimeIndex, finishTime[sTimeIndex], turnAroundTime[sTimeIndex]);
  }

  //TESTING
  /*for(i = 0; i < numprocs; i++) {
    printf("\navail[%d]: %d\n", i, avail[i]);
    printf("complete[%d]: %d\n", i, complete[i]);
  }
  */
  
  printTable(numprocs, procs, finishTime, turnAroundTime, 1);
}

void hrrn(int numprocs, proc_t *procs)
{
  
  //Variables for holding the data that will be accumulated (totals)
  int i, current_no, clock, tot_service_time;
  double tot_wait_time, tot_turnaround_time;

  //These two variables hold the hrrn ratio and a temp value for that ratio for comparison
  float hrr, temp;
  // Two arrays to hold the turnaround times for each process and the processes that have finished running (so we don't continue to count for them)
  int turnaround_time[numprocs], completed_procs[numprocs], finish_time[numprocs];

  for(i = 0; i < numprocs; i++) {

	turnaround_time[i] = 0;			//Setting each value to zero
	completed_procs[i] = 0;			// making sure completed array is initialized to zero
	tot_service_time = procs[i].service_time + tot_service_time;
}

  clock = procs[0].arrival_time;		//This should = 0

  //We calculated the total service time, so the clock shouldn't have to run afterward
  while(clock < tot_service_time) {

	//initialize to zero
	hrr = 0;

	for(i = 0; i < numprocs; i++) {

		//If process is not complete
		if(procs->arrival_time <= clock && completed_procs[i] != 1) {
			//This is the HRRN formula at work
			temp = (procs[i].service_time + (clock - procs[i].arrival_time)) / procs[i].service_time;
			//Logic for if there is a HRR that is lower, so that it becomes next
			if(hrr < temp) {
				//Swap logic
				hrr = temp;
				current_no = i;
			}
		}

	clock += procs[current_no].service_time;
	
	procs[current_no].wait_time = clock - procs[current_no].arrival_time - procs[current_no].service_time;

	turnaround_time[current_no] = clock - procs[current_no].arrival_time;

	completed_procs[current_no] = 1;	// 1 == the process is complete

	finish_time[current_no] = clock;

	tot_wait_time = procs[current_no].wait_time + tot_wait_time;

	tot_turnaround_time = turnaround_time[current_no] + tot_turnaround_time;
	}
  }
  //Printing the results
  printTable(numprocs, procs, finish_time, turnaround_time, 2);

  return;

}


int main(int argc, char** argv)
{
  int numprocs, seed;
  proc_t *procs;   // array of procs with service time and arrival time

  if (argc < 3) {
    fprintf(stderr, "USAGE: %s numprocs seed\n", argv[0]);
    return 1;
  }

  numprocs = atoi(argv[1]);
  seed = atoi(argv[2]);

  //create an array of numprocs randomly generate (arrival time, service time)
  procs = procs_random_create(numprocs, seed, INTER_ARRIVAL_TIME, SERVICE_TIME);
  
  // Uncomment the next block of code and comment the line above if
  // you want to read the input from a file instead of generating
  // random values. This can be useful for debugging your code with
  // known inputs.  The file, "book_example.txt", contains an example
  // from the book.

  
  /*if ((procs = procs_read("book_example.txt", &numprocs)) == NULL) {
    fprintf(stderr, "Error reading procs array\n");
    return 2;
  }*/
  

  printf("procs array:\n");
  printf("(arrival time, service time)\n");
  procs_print(procs, numprocs);

  /* Either call your implemented functions for performing FCFS, SPN,
     and HRRN, or insert your code here to perform these algorithms.
     My recommendation is to write one or more functions for these
     algorithms in order for your main to be more readable and
     manageable.
  */

  printf("\n");
  fcfs(numprocs, procs);
  printf("\n");
  spn(numprocs, procs);
  printf("\n");
  hrrn(numprocs, procs);
  
  

  free(procs);   // procs array was dynamically allocated, so free up

  return 0;
}



