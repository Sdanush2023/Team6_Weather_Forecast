#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

// Structure for storing temperature, humidity, and wind data
struct WeatherData {
  float temperature;
  float humidity;
  float wind;
};

void error_handler(char *message);


int main(int argc, char *argv[]) {

  if(argc!=4)
   {
    printf("ERROR: Enter TEMP (celsius), HUMIDITY (percentage), WIND (meter/second)");
    exit(1);
   }
  else{
  printf("Data sent to shared memory.\n");
  }
  
  size_t size = sizeof(struct WeatherData);

  int shmid = shmget((key_t)2000, size, IPC_CREAT | 0666);
  if (shmid == -1) {
    error_handler("shmget\n");
  }

  struct WeatherData *shared_data = shmat(shmid, NULL, 0);

  shared_data->temperature = atof(argv[1]);
  shared_data->humidity = atof(argv[2]);
  shared_data->wind = atof(argv[3]);

  if (shmdt(shared_data) == -1) {
   error_handler("shmdt\n");
    exit(EXIT_FAILURE);
  }
  return 0;
}


void error_handler(char *message) {
  perror(message);
  exit(1); // unsucessful process termination
}

