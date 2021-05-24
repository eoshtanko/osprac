#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

    int main(void){
        int count = 0;
        char pathName[] = "a";
        char pathNameNow[1024];
        char pathNameBefore[1024];

        int fd = open(pathName,O_RDWR|O_CREAT,0666);
        if (fd < 0){
            printf("Imposible to create a\n");
        }
        if(close(fd) != 0){
            printf("Imposible to close a\n");
        }
        sprintf(pathNameNow, "a%d", count);
        symlink(pathName, pathNameNow);

        while((fd = open(pathNameNow,O_RDONLY, 0666))>= 0){
            count++;
            strncpy(pathNameBefore, pathNameNow, sizeof(pathNameBefore));
            sprintf(pathNameNow, "a%d", count);
            symlink(pathNameBefore,pathNameNow);
            if(close(fd)< 0){
                printf("Error while closing file\n");
                exit(-1);
            }
        }
        printf("%d\n", count);
        return 0;
    }
