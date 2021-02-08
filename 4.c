#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]){
pid_t pid;
pid = fork();
if(pid == -1){
printf("Error");
} else if(pid == 0){
(void) execle("/bin/cat","/bin/cat" ,"1.c" ,0, envp);
}else{
printf("Parent !!!");
}
return 0;
}
