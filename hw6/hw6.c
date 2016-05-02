#include <stdio.h>
#include "dpsim.h"

/**************************************************

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread 
	- If the return value != 0, then display an error message and 
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {


	// ---------------------------------------
	// TODO: you add your implementation here
	
	//Creating the variables necessary
	pthread_t process_thread;
	int status;
	
	//Creating the main thread
		//Logic for threads creation
		//if FAIL --> 0
	//Call th_main to use that thread function
	if ( pthread_create( &process_thread, NULL, th_main, NULL ) != 0 ) {
		printf("Error in creating thread in main... Exiting.\n");
		status = 1;
		exit(status);
	}

	if ( pthread_join( process_thread, NULL ) != 0 ) {
		printf("Error in joining thread in main... Exiting.\n");
		status = 2;
		exit(status);
	}

	printf("Thread successfully joined. Status = %i\n", status);

	return 0;

} // end main function
