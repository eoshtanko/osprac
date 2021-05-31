#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
  if(nsig == SIGQUIT){
    printf("\nCTRL-4 was pressed!\n");
  }
  else if(nsig == SIGINT){
    printf("\nCTRL-C was pressed!\n");
  }
}

int main(void) {
  //
  // Set the process response to the SIGINT signal
  //
  (void)signal(SIGINT, my_handler);
  (void)signal(SIGQUIT, my_handler);
  //
  // From this point, the process will print SIGINT message.
  //
  while(1);
  return 0;
}
