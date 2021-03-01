#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
char *array; // Pointer to shared memory
int shmid; // IPC descriptor for the shared memory
int new = 1; // Initialization flag for the elements of the array
char pathname[] = "3.c"; // The file name used to generate the key.


// A file with this name must exist in the current directory.
key_t key; // IPC key
if ((key = ftok(pathname,0)) < 0) {
printf("Can\'t generate key\n");
exit(-1);
}

//
// Trying to create a shared memory exclusively for the generated key, that is,
// if it already exists for this key, the system call will return a negative value.
// The memory size is defined as the size of an array of three integer variables,
// access rights 0666 - reading and writing are allowed for everyone.
//
if ((shmid = shmget(key, 3000*sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
//
// In case of an error, try to determine: whether it arose due to the fact
// that the shared memory segment already exists or for some other reason.
//
if (errno != EEXIST) {
printf("Can\'t create shared memory\n");
exit(-1);
} else {
//
// If because the shared memory already exists,
// try to get its IPC descriptor and,
// if successful, reset the Initialization flag.
//
if ((shmid = shmget(key, 2500*sizeof(char), 0)) < 0) {
printf("Can\'t find shared memory\n");
exit(-1);
}
new = 0;
}
}

//
// Try to map shared memory to the address space of the current process.
// Note that for proper comparison we explicitly convert the value -1 to a pointer to an integer.
//
if ((array = (char*)shmat(shmid, NULL, 0)) == (char*)(-1)) {
	printf("Can't attach shared memory\n");
	exit(-1);
}

//
// Depending on the value of the flag,
// we either initialize the array or increment the corresponding counters.
//

if (new) {
char c;
int i = 0;
FILE *file = fopen("3.c", "r");
if (file) {
while ((c = getc(file)) != EOF){
array[i] = c;
i++;
}
fclose(file);
}
}


//
// Print the new values of the counters,
// remove the shared memory from the address space of the current process, and exit.
//

if (shmdt(array) < 0) {
printf("Can't detach shared memory\n");
exit(-1);
}

return 0;
}
