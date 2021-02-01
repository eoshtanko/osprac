#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
int pid, ppid;
pid = (int)getpid();
ppid = (int)getppid();
printf("My id - %d, Parent id - %d\n", pid, ppid);
return 0;
}
