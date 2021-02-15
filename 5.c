#include <sys/types.h> 
 #include <unistd.h> 
 #include <stdio.h> 
 #include <stdlib.h> 
 int main() 
 { 
 int fd[2],sec[2], result; 
 size_t size; 
 char resstring[14]; 
 if (pipe(fd) < 0) { 
 printf("Can\'t first open pipe\n"); 
 exit(-1); 
 } 

 if (pipe(sec) < 0) { 
 printf("Can\'t second open pipe\n"); 
 exit(-1); 
 } 

 result = fork(); 
 if (result < 0) { 
 printf("Can\'t fork child\n"); 
 exit(-1); 
 } 

 if (result > 0) { 
 /* Parent process */ 

 if (close(fd[0]) < 0) { 
 printf("parent: Can\'t close reading side of pipe\n"); exit(-1); 
 } 

 if (close(sec[1]) < 0) { 
 printf("parent: Can\'t close writing side of pipe\n"); exit(-1); 
 } 

 size = write(fd[1], "ABCDEFGHIGKLMN", 14); 

 if (size != 14) { 
 printf("Can\'t write all string to pipe\n"); 
 exit(-1); 
 } 

 if (close(fd[1]) < 0) { 
 printf("parent: Can\'t close writing side of pipe\n"); exit(-1); 
 } 

 size = read(sec[0], resstring, 14); 
 if (size < 0) { 
 printf("Can\'t read string from pipe\n"); 
 exit(-1); 
 } 
   
 printf("Parent exit, result:%s\n", resstring); 
 if (close(sec[0]) < 0) { 
 printf("parent: Can\'t close reading side of pipe\n"); exit(-1); 
 } 

 } else { 
 /* Child process */ 

 if (close(sec[0]) < 0) { 
 printf("parent: Can\'t close reading side of pipe\n"); exit(-1); 
 } 

 size = write(sec[1], "ABCDEFGHIGKLMN", 14); 

 if (close(fd[1]) < 0) { 
 printf("child: Can\'t close writing side of pipe\n"); exit(-1); 
 } 
 size = read(fd[0], resstring, 14); 
 if (size < 0) { 
 printf("Can\'t read string from pipe\n"); 
 exit(-1); 
 } 
 printf("Child exit, result:%s\n", resstring); 
 if (close(fd[0]) < 0) { 
 printf("child: Can\'t close reading side of pipe\n"); exit(-1); 
 } 
 } 

 return 0; 
 }
