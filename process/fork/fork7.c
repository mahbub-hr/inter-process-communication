#include <stdio.h> 
#include <unistd.h> 

int main( void ) {
	char *argv[3] = {"Command-line", ".", NULL};

	int pid = fork();

	if ( pid == 0 ) {
		execvp( "find", argv );
	}

	/* Put the parent to sleep for 10 seconds--let the child finished executing */
	sleep(10);

	printf( "Finished executing the parent process\n"
	        " - the child won't get here--you will only see this once\n" );

	return 0;
}
