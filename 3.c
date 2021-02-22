#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
 int fd;
 size_t size;

 (void)umask(0);

 if (mknod("FIFO_TEST.fifo", S_IFIFO | 0666, 0) < 0) {
   printf("Cant create FIFO\n");
   exit(-1);
 }

if ((fd = open("FIFO_TEST.fifo", O_WRONLY)) < 0) {
  printf("Cant open FIFO\n");
  exit(-1);
 }

 size = write(fd, "Hello, world!", 14);

 if (size != 14) {
  printf("Cant write all str in FIFO\n");
  exit(-1);
 }
 return 0;
}
