#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include<stdlib.h>

void error_handler(char *message) {
  perror(message);
  exit(1); // unsucessful process termination
}


int main() {
  int sval;
  // sem_unlink("/semaph");
  sem_t *semaphore;
  semaphore = sem_open("/semaphore", O_CREAT | O_RDWR, 0666, 1);

  if (semaphore == NULL) // SEM_FAILED == NULL
  {
    error_handler("sem_open fail\n");
  } else {
    printf("\n sem_open success");
  }
  int n = sem_getvalue(semaphore, &sval);
  if (n == -1) {
    printf("system call failed\n");
  }
}
