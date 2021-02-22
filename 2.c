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
 char resstring[14];

 if ((fd = open("FIFO_TEST.fifo", O_RDONLY)) < 0) {
  printf("Can\'t open FIFO\n");
  exit(-1);
 }

 size = read(fd, resstring, 14);

 if (size < 0) {
  printf("Cant read str\n");
  exit(-1);
 }

 printf("%s\n", resstring);

 if (close(fd) < 0) {
  printf("Cant close FIFO\n");
  exit(-1);
 }
  return 0;
}
