#include <stdio.h>    /* for printf */
#include <stdlib.h>   /* for string to integer conversion, random numbers */
#include "mem.h"

/*
  The main program will accept four paramemters on the command line.
  The first parameter is the memory size.  The second parameter is the
  duration of the each simulation run.  The third parameter is the
  number of times to repeat each experiment (ie. number of runs). The
  fourth parameter is a random number seed. Here is an example command
  line:

  ./hw7 1000 3000 100 1235

  This means that your program should initialize physical memory to
  1,000 units, perform 100 runs with each run taking 3000 units of
  time, and the random number generator should be seeded (one time)
  with the value 1235.
*/

int main(int argc, char** argv)
{  
	//Intitializing some variables from command line
	int memory_size, simulation_duration, num_runs, rand_seed;
	int request_size, duration;

	//Declaring varibales to hold data from the simulation
	int first_probes, first_frags, first_avg_probes, first_avg_frags, first_avg_fails; 
	int first_total_probes = 0; 
	int first_total_fails = 0; 
	int first_total_frags = 0;

	int next_probes, next_frags, next_avg_probes, next_avg_frags, next_avg_fails; 
	int next_total_probes = 0;
	int next_total_fails = 0;
	int next_total_frags = 0;

	int best_probes, best_frags, best_avg_probes, best_avg_frags, best_avg_fails;
	int best_total_probes = 0;
	int best_total_fails = 0;
	int best_total_frags = 0;

	memory_size = ( int )strtol(argv[1], 0, 10);
	simulation_duration = ( int )strtol(argv[2], 0, 10);
	num_runs = ( int )strtol(argv[3], 0, 10);
	rand_seed = ( int )strtol(argv[4], 0, 10);

	//Gotta have i, j - to count!
	int i, j;

	//Calling the mem_init function
	mem_init(memory_size);

	//seeding the random number generator
	srand(rand_seed);

	//Starting the simulation through the loop
	for ( i = 0; i < simulation_duration; i++ ) {
		for ( j = 0; j < num_runs; j++ ) {
			request_size = rand() % MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;
			duration = rand() % MAX_DURATION - MIN_DURATION;
			//Now to actually use the mem_allocate function
			first_probes = mem_allocate( FIRSTFIT, request_size, duration);
			//Logic for if the simulation was unsuccessful
			if ( first_probes == -1 ) {
				first_total_fails += 1;
			}
			else if ( first_probes != -1 ) {
				first_total_probes += first_probes;
			}
			
			//now to retrieve the number of fragments
			first_frags = mem_fragment_count(MIN_REQUEST_SIZE - 1); // -1 because we dont want to index out

			first_total_frags += first_frags;

			//storing the average values for printout later
			first_avg_frags = first_total_frags/simulation_duration;
  			first_avg_probes = first_total_probes/simulation_duration;
  			first_avg_fails = first_total_fails/simulation_duration;

			mem_single_time_unit_transpired();
		}
		
		mem_clear();

		//Now for the next fit situation
		for ( j = 0; j < num_runs; j++ ) {
			request_size = rand() % MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;
			duration = rand() % MAX_DURATION - MIN_DURATION;
			//Now to actually use the mem_allocate function
			next_probes = mem_allocate( NEXTFIT, request_size, duration);
			//Logic for if the simulation was unsuccessful
			if ( next_probes == -1 ) {
				next_total_fails += 1;
			}
			else if ( next_probes != -1 ) {
				next_total_probes += next_probes;
			}
			
			//now to retrieve the number of fragments
			next_frags = mem_fragment_count(MIN_REQUEST_SIZE - 1); // -1 because we dont want to index out

			next_total_frags += next_frags;

			//storing the average values for printout later
			next_avg_frags = next_total_frags/simulation_duration;
  			next_avg_probes = next_total_probes/simulation_duration;
  			next_avg_fails = next_total_fails/simulation_duration;

			mem_single_time_unit_transpired();
		}
		
		mem_clear();

		//Now for the best fit strategy
		for ( j = 0; j < num_runs; j++ ) {
			request_size = rand() % MAX_REQUEST_SIZE - MIN_REQUEST_SIZE;
			duration = rand() % MAX_DURATION - MIN_DURATION;
			//Now to actually use the mem_allocate function
			best_probes = mem_allocate( BESTFIT, request_size, duration);
			//Logic for if the simulation was unsuccessful
			if ( best_probes == -1 ) {
				best_total_fails += 1;
			}
			else if ( best_probes != -1 ) {
				best_total_probes += best_probes;
			}
			
			//now to retrieve the number of fragments			
			best_frags = mem_fragment_count(MIN_REQUEST_SIZE - 1); // -1 because we dont want to index out

			best_total_frags += best_frags;

			//storing the average values for printout later
			best_avg_frags = best_total_frags/simulation_duration;
  			best_avg_probes = best_total_probes/simulation_duration;
  			best_avg_fails = best_total_fails/simulation_duration;

			mem_single_time_unit_transpired();
		}
		
		mem_clear();
	}

	//Printing the data
	printf("\t________FIRST________\n");
  	printf("\tAvg Probes: %i\n", first_avg_probes);
  	printf("\tAvg Fails %i\n", first_avg_fails);
  	printf("\tAvg Frags: %i\n", first_avg_frags);

	printf("\t________NEXT________\n");
  	printf("\tAvg Probes: %i\n", next_avg_probes);
  	printf("\tAvg Fails %i\n", next_avg_fails);
  	printf("\tAvg Frags: %i\n", next_avg_frags);

	printf("\t________BEST________\n");
  	printf("\tAvg Probes: %i\n", best_avg_probes);
  	printf("\tAvg Fails %i\n", best_avg_fails);
  	printf("\tAvg Frags: %i\n", best_avg_frags);

	//Freeing the memory
	mem_free();
	  
	return 0;
}
