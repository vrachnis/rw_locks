#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "_rw_locks.h"

void *init_function(void *ptr);
int counter = 5;

int main(int argc, char **argv)
{
  struct rw_lock_t *rwvar=NULL;

  if (rw_init(rwvar)) {
    perror("Initialization failed");
    exit(1);
  }

  pthread_t thread1, thread2;
  char *message1 = "Thread 1";
  char *message2 = "Thread 2";
  int ret1, ret2;

  /* Create the independent threads
  calling the init function*/

  ret1 = pthread_create(&thread1, NULL, init_function, (void *) message1);
  ret2 = pthread_create(&thread2, NULL, init_function, (void *) message2);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Thread 1 returns: %d\n", ret1);
  printf("Thread 2 returns: %d\n", ret2);

  if (rw_destroy(rwvar)) {
    perror("Destruction failed");
    exit(1);
  }

  return 0;
}

void * init_function(void *ptr)
{
  char *message=NULL;
  message = (char *) ptr;
  printf("%s Initialized.\n", message);

  return 0;
}
