#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<semaphore.h>
#include<unistd.h>
#include<fcntl.h>

#define SIZE 1024

void send_file_data(FILE *fp, int sockfd, struct sockaddr_in addr) {
  int n;
  char buffer[SIZE];

  // Sending the data
  while (fgets(buffer, SIZE, fp) != NULL) {
    printf("[SENDING] Data: %s", buffer);

    n = sendto(sockfd, buffer, SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (n == -1) {
      perror("[ERROR] sending data to the server.");
      exit(1);
    }
    bzero(buffer, SIZE);
  }

  // Sending the 'END'
  strcpy(buffer, "END");
  sendto(sockfd, buffer, SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));

  fclose(fp);
}

void error_handler(char *message) {
  perror(message);
  exit(1); // unsucessful process termination
}

int main(void) {
   
   int sval;
   
   sem_t *sem;// pointer
   
   sem = sem_open("/semaphore",  O_RDWR); 
   
   if(sem==SEM_FAILED)
    {
      error_handler("sem open failed\n");
    }
   else{
      printf("\nSem_open success\n");
       }
       
   sem_getvalue(sem,&sval); // sval = 1
   
   printf("\nsemaphore value = %d\n", sval); //1
   
  // Defining the IP and Port
  char *ip = "127.0.0.1";
  const int port = 8080;

  // Defining variables
  int server_sockfd;
  struct sockaddr_in server_addr;
  char *filename = "file.txt";
  FILE *fp = fopen(filename, "r");

  // Creating a UDP socket
  server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_sockfd < 0) {
    error_handler("[ERROR] socket error\n");
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Reading the text file
  if (fp == NULL) {
     error_handler("[ERROR] reading the file\n");
  }

  sem_wait(sem);
  
  sem_getvalue(sem,&sval); // sval = 1

  printf("\nsemaphore value = %d\n", sval); //1
   
  // Sending the file data to the server
  send_file_data(fp, server_sockfd, server_addr);

  printf("[SUCCESS] Data transfer complete.\n");
  printf("[CLOSING] Disconnecting from the server.\n");

  close(server_sockfd);
  
  sem_post(sem);//increment semaphore value
  
  sem_getvalue(sem,&sval); // sval = 1

  printf("\nsemaphore value = %d\n", sval); //1

  return 0;
}
