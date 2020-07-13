#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include "semutil.h"

int main(int argc, char *argv[])
{
	int i;
	sem_id = semget((key_t) 1234, 1, 0666 | IPC_CREAT);
	int pid = fork();
	if (pid != 0) {
		if (!set_semvalue()) {
			printf("Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
		sleep(2);
	}
	for(i = 0; i < 5; i++) {
		if (!semaphore_p()) exit(EXIT_FAILURE);
		printf("hello");
		fflush(stdout);
		sleep(4);
		printf("world");
		fflush(stdout);
		if (!semaphore_v()) exit(EXIT_FAILURE);		
	}
	printf("\n%d - finished\n", getpid());
	if (pid != 0) {
		sleep(6);
		del_semvalue();
	}
	exit(EXIT_SUCCESS);
}
