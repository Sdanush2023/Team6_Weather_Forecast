#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

static char img[100];

struct WeatherData {
  float temperature;
  float humidity;
  float wind;
};

void writeToFile(struct WeatherData *data);
char *makePrediction(struct WeatherData *data);

int main() {
  sem_t *semaphore;
  int sval;
  semaphore = sem_open("/semaphore", O_RDWR);

  size_t size = sizeof(struct WeatherData);
  int shmid = shmget((key_t)2000, size, 0666);

  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  struct WeatherData *shared_data = shmat(shmid, NULL, 0);

  if (semaphore == NULL) // SEM_FAILED == NULL
  {
    perror("sem_open fail\n");
    return -1;
  } else {
    printf("\n sem_open success");
  }

  sem_getvalue(semaphore, &sval);
  printf("\n semaphore val =(%d)\n", sval);
  sem_wait(semaphore);

  sem_getvalue(semaphore, &sval);
  printf("\n semaphore val after entering crtical section =(%d)\n", sval);

  // Write received data and prediction result to file
  writeToFile(shared_data);

  sem_post(semaphore);
  return 0;
}

char *makePrediction(struct WeatherData *data) {

  static char mpred[100];
  if ((data->temperature > 25.0 && data->temperature <= 50.0) &&
      data->humidity > 0.0 && data->humidity < 60.0) {
    if (data->wind > 0.0 && data->wind < 8.0) {
      printf("Prediction: Sunny\n");
      strcpy(mpred, "Sunny");
      strcpy(img, "\u2600");
    } else if (data->wind >= 8.00) {
      printf("Prediction: Sunny and Windy\n");
      strcpy(mpred, "Sunny and Windy");
      strcpy(img, "\u2600 \U0001F32A");
    } else {
      printf("!! Prediction Failed !!\n");
      strcpy(mpred, "!! Prediction Failed !!");
    }
  } else if ((data->temperature > 15.0 && data->temperature <= 25.0) &&
             data->humidity > 0.0 && data->humidity < 60.0) {
    if (data->wind > 0.0 && data->wind < 15.0) {
      printf("Prediction: Rainy\n");
      strcpy(mpred, "Rainy");
      strcpy(img, "\u2614");
    } else if (data->wind >= 15.0) {
      printf("Prediction: Rainy and chance of storm\n");
      strcpy(mpred, "Rainy and chance of storm ");
      strcpy(img, "\u2614 \u2601\u26A1");
    } else {
      printf("!! Prediction Failed !!\n");
      strcpy(mpred, "!! Prediction Failed !!");
    }

  } else if ((data->temperature > 15.0 && data->temperature <= 25.0) &&
             data->humidity > 0.0 && data->humidity < 60.0) {
    if (data->wind > 0.0 && data->wind < 15.0) {
      printf("Prediction: Cloudy\n");
      strcpy(mpred, "Cloudy");
      strcpy(img, "\u2601");
    } else if (data->wind >= 15.0) {
      printf("Prediction: Cloudy and Windy\n");
      strcpy(mpred, "Cloudy and Windy");
      strcpy(img, "\u2601 \U0001F32A");
    } else {
      printf("!! Prediction Failed !!\n");
      strcpy(mpred, "!! Prediction Failed !!");
    }
  } else if ((data->temperature > 0.0 && data->temperature <= 15.0) &&
             data->humidity > 0.0 && data->humidity < 60.0) {
    if (data->wind > 0.0 && data->wind < 15.0) {
      printf("Prediction: Winter\n");
      strcpy(mpred, "Winter");
      strcpy(img, "\u2744");
    } else if (data->wind >= 15.0) {
      printf("Prediction: Winter and Windy\n");
      strcpy(mpred, "Winter and Windy");
      strcpy(img, "\u2744\U0001F32A");
    } else {
      printf("!! Prediction Failed !!\n");
      strcpy(mpred, "!! Prediction Failed !!");
    }
  } else {
    printf("!! Prediction Failed !!\n");
    strcpy(mpred, "!! Prediction Failed !!");
  }
  return mpred;
}

void writeToFile(struct WeatherData *data) {

  FILE *file = fopen("file.txt", "a");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  char *prediction = makePrediction(data);

  fprintf(file, "Timestamp: %s %s\n", __TIME__, __DATE__);
  fprintf(file, "Temperature: %.2f\n", data->temperature);
  fprintf(file, "Humidity: %.2f\n", data->humidity);
  fprintf(file, "Wind: %.2f\n", data->wind);
  fprintf(file, "Weather Prediction : %s", prediction);
  fprintf(file, ": %s\n", img);
  fclose(file);
}

