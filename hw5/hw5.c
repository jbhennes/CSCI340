/*=============================================================

Author: Jake Hennessy
CSCI 340
Homework 5
Feb 29, 2016

==============================================================*/


#include <stdio.h>         // for printf()
#include <pthread.h>       // for pthread_xxx() routines
#include <time.h> 	   // Time library for the timespec struct

#include "mcsim.h" // monte carlo simulation function prototypes

#define DEBUG 0

double gcount;    // global counter -- a double to handle large sums (billions+)

long numits;      // global variable for number of iterations (see step 3 below)


  /*

    ---------------------------------------------------------------------
    Step-by-Step procedure for Monte carlo simulator to estimate PI
    ---------------------------------------------------------------------
    1. Create the following variables:
       - pointer to an array of pthread structures
       - counting variables for loops
       - thread id
       - exit status of joined thread
       - number of threads to run
       - maximum number of threads (hint: rlimit)
       - nanosecond start and stop timespec structs
    2. Get number of threads input by user from argv[1]
    3. Get number of iterations input by user from argv[2]
    4. Get maximum number of threads the OS can create (man getrlimit function)
    5. If number of threads > maximum OS threads then goto 14, else goto step 6
    6. Allocate array of pthread structures using number of threads from step 2
    7. Get start time of simulation and store in timespec struct
    8. Create a pthread for each position in pthread array created in step 6
       - if an error occurs during thread creation exit simulation immediately
    9. Join each pthread created in the pthread array
       - if an error occurs during thread join exit simulation immediately
    10. Get stop time of simulation and store in timespec struct
    11. Print out:
       - value of PI using 6 decimal places
       - value stored in global variable gcount using 0 decimal places
       - time in seconds to run the simulation using 4 decimal places (hint: use mydifftime function)
    12. Free memory allocated for pthread array
    13. Call pthread terminate function (hint: pthread_exit function)
    14. exit simulation program */

int main(int argc, char** argv)
{
  gcount = 0;

  //printf("gcount ok\n");

  //Defining the variables to be used in step one
  pthread_t * thread_array;
  int i = 0; 
  long thread_id = 0; 
  long thread_exit_status = 0;
  int num_total_threads;
  int error_num; 
  struct rlimit max_num_threads;
  struct timespec start; 
  struct timespec stop;
  double total_time;

  //In case there weren't enough arguments (via argc)
  if ( argc < 3 ) {
     printf("Not enough arguments provided - please enter number of threads and iteration arguments, exiting program");
     exit( -1 ); 
  }
  
  //printf("vars defined ok\n");

  //Now we are implementing the operations
  //Getting the number of threads from user input in argv[1]
  num_total_threads = atoi( argv[1] );

  //printf("num_total_threads = %d\n", num_total_threads);

  //Get number of iterations input by user from argv[2]
  // NOte: strtol( the args to convert, endpoint, and base-10)
  numits = strtol( argv[2], NULL, 10 );

  //printf("numits = %ld\n", numits);
  
  //Get maximum number of threads the OS can create
  getrlimit(RLIMIT_NPROC, &max_num_threads);

  //printf("getrlimit succeeded!");

  //If number of threads > maximum OS threads then we must exit the program 
  if ( num_total_threads > max_num_threads.rlim_cur ) {
      //Exit the program because num_total_threads is 0
      printf("The number entered has exceeded the maximum allowed number of threads by the CPU ... exiting.");
      exit(-1);
  }

  else { 
      //Here allocate space based on the # of commands the user entered
      thread_array = (pthread_t *)malloc(sizeof(pthread_t) * num_total_threads);

      //Next we want to start the clock by using the clock_gettime system function
      clock_gettime(CLOCK_REALTIME, &start);

      //Create a pthread for each position in pthread array created by using the malloc
      for ( i = 0; i < num_total_threads; i++ ) {

          //Testing for an error value to be returned by the pthread_create proces logic for if the thread created returned an error
	  if ( ( error_num = pthread_create(&(thread_array[i]), NULL, &th_routine, (void *) thread_id++ ) ) != 0 ) {
	     printf("ERROR: The thread created at position %d created an error and caused the program to quit.\n", i);
	     exit( error_num );
          }
      }

      //Joining pthreads for each position in pthread array 
      //Also note we reset i
      for ( i = 0; i < num_total_threads; i++ ) {

	  //Once again we test for the error value of zero to be returned by the pthread_join process logic for if the value returned was 0 -- indicating a error
	  if ( ( error_num = pthread_join( thread_array[i], NULL) ) != 0 ) {
	     printf("ERROR: THe thread joined at position %d created an error and caused the program to quit.\n", i );
	     exit( error_num );
	  }
      }

      //Getting the time that the threading stops
      clock_gettime(CLOCK_REALTIME, &stop);

      //getting the differences in the two times, to get total time elapsed
      total_time = mydifftime(&start, &stop);

      //Print statements
	printf("=================== Monte Carlo Method ===================\n\t\tPI = %.6f\n\t\tvalue of gcount: %.0f\n\t\telapsed time in seconds: %.4f\n", ((4 * gcount)/(numits * num_total_threads)), gcount, (total_time / 1000000000));
  }
  
  //Freeing the thread array that we malloc'ed
  free( thread_array );

  //termintaing the thread
  pthread_exit( (void *) thread_exit_status );

  //exit simulation with the return statement below.
  return 0;

} // end main function

/*    ---------------------------------------------------------------------
    Example input and output of Monte carlo simulator to estimate PI
    ---------------------------------------------------------------------

    # ./hw5 300 100  // i.e. run simulatior using 300 threads and 100 iterations

    monte carlo value of PI = 3.141200
    value of count = 23559
    time in seconds = 0.0761
  */
