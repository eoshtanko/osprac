#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(){
	int fd;
	size_t  size;
	char resstring[14];
	char name[] = "message.fifo";
	(void)umask(0);
	
	if (mkfifo(name, 0666) < 0){
		printf("fail to open fifo");
		exit(-1);
	};

	if ((fd = open(name, O_WRONLY)) < 0){
		printf("Cant open FIFO: writting\n");
      		exit(-1);
	}

	size = write(fd, "Hello, world!", 14);
	
	if (size != 14) {
		printf("Cant write all string to FIFO\n");
		exit(-1);
    	}
	if (close(fd) < 0) {
		printf("parent: Cant close FIFO\n"); 
		exit(-1);
	}

	return 0;
}
