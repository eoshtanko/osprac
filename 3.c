#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(){
pid_t pid;
pid = fork();

if(pid == -1){
printf("Error");
} else if(pid == 0){
printf("Child !!!");
}else{
printf("Parent !!!");
}
return 0;
}
