#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

int main(void) {
  int pipefds[2];
  int number; 
  int i;
  int flag = 0;

  //Pipe stuff, please ignore
  if(pipe(pipefds) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
 
  pid_t pid = fork();
 
  if(pid == 0) { // in child process
	  
	//SIGINT stuff, leave as is
	void sigint_handler(int sig);
	if (signal(SIGINT, sigint_handler) == SIG_ERR){
		perror("signal");
		exit(1);
	}
	
	//User input
	printf("Enter number to check if prime or not: ");
	scanf("%d", &number);

    close(pipefds[0]); // close read fd
    write(pipefds[1], &number, sizeof(number)); // write user input to pipe
    exit(EXIT_SUCCESS);
  }
 
  if(pid > 0) { // in parent process
    wait(NULL); // wait for child process to finish
    close(pipefds[1]); // close write fd
    read(pipefds[0], &number, sizeof(number)); // read user input from pipe
	
	//Prime calculation
	for (i = 2; i <= number / 2; ++i) {

        // condition for non-prime
        if (number % i == 0) {
            flag = 1;
            break;
        }
    }
    if (number == 1) {
        printf("1 is neither prime nor composite.\n");
    }
    else {
        if (flag == 0)
            printf("%d is a prime number.\n", number);
        else
            printf("%d is not a prime number.\n", number);
    }
	
    close(pipefds[0]); // close read fd
  }
  return EXIT_SUCCESS;
}

void sigint_handler(int sig) 
{
  printf("Process terminated. Press ENTER to return.\n");
}
