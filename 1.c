#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
  int fd[2];
  int count = 0;
  char byte[1];

  if(pipe(fd) < 0){
     printf("Cant open pipe\n");
     exit(-1);
  }
  if(fcntl(fd[1], F_SETFL, O_NONBLOCK) < 0){
    printf("Cant make pipe O_NONOBLOK\n");
    exit(-1);
  }
  while(write(fd[1], byte, 1)>=0){
    count++;
  }
  printf("Size of the pipe - %d bytes", count/1024);
  return 0;
}
