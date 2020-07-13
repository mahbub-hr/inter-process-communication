
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

static int sem_id;

static int set_semvalue(void)
{
	if (semctl(sem_id, 0, SETVAL, 1) == -1) return(0);
	return(1);
}

static void del_semvalue(void)
{
	if (semctl(sem_id, 0, IPC_RMID) == -1)
		printf("Failed to delete semaphore\n");
}

static int semaphore_p(void)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		printf("semaphore_p failed\n");
		return(0);
	}
	return(1);
}

static int semaphore_v(void)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		printf("semaphore_v failed\n");
		return(0);
	}
	return(1);
}
