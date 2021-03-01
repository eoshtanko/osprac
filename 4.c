#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
	char* array; // Pointer to shared memory
	int shmid; // IPC descriptor for the shared memory
	int new = 1; // Initialization flag for the elements of the array
	char pathname[] = "3.c"; // The file name used to generate the key.
	key_t key;

	if ((key = ftok(pathname, 0)) < 0) {
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((shmid = shmget(key, 2500 * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
		if (errno != EEXIST) {
			printf("Can\'t create shared memory\n");
			exit(-1);
		}
		else {
			if ((shmid = shmget(key, 2500 * sizeof(char), 0)) < 0) {
				printf("Can\'t find shared memory\n");
				exit(-1);
			}
			new = 0;
		}
	}

	//
	if ((array = (char*)shmat(shmid, NULL, 0)) == (char*)(-1)) {
		printf("Can't attach shared memory\n");
		exit(-1);
	}
	//
	// Depending on the value of the flag,
	// we either initialize the array or increment the corresponding counters.
	//
	for (int i = 0; i < 2500; i++) {
		printf("%c", array[i]);
	}

	//
	// Print the new values of the counters,
	// remove the shared memory from the address space of the current process, and exit.
	//

	if (shmdt(array) < 0) {
		printf("Can't detach shared memory\n");
		exit(-1);
	}

	shmctl(shmid, IPC_RMID, 0);


	return 0;
}
