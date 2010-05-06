#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "_rw_locks.h"

#define NR_THREADS 16

void *init_function(void *ptr);
pthread_mutex_t cmut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wmut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int counter = 5;

struct rw_lock_t *rwvar=NULL;

int main(int argc, char **argv)
{
  if (rw_init(&rwvar)) {
    perror("Initialization failed");
    exit(1);
  }

  printf("readers: %d\nwriters: %d\n", rwvar->readers, rwvar->writers);

  pthread_t threads[NR_THREADS];
  int i;
  char role[NR_THREADS]={'r','w','r','r','r','r','r','r','w','r','r','r','r','r','r','r'};
  int ret[NR_THREADS];

  /* Create the independent threads
  calling the init function*/

  for (i=0 ; i<NR_THREADS ; i++) {
    ret[i] = pthread_create(&threads[i], NULL, init_function, (void *) role[i]);
  }

  for (i=0 ; i<NR_THREADS ; i++) {
    pthread_join(threads[i], NULL);
  }

  /* printf("Thread 1 returns: %d\n", ret1); */
  /* printf("Thread 2 returns: %d\n", ret2); */

  if (rw_destroy(&rwvar)) {
    perror("Destruction failed");
    exit(1);
  }

  return 0;
}

void * init_function(void *ptr)
{
  /* char *message=NULL; */
  /* message = (char *) ptr; */
  /* printf("%s Initialized.\n", message); */
  char role;
  role = (char) ptr;
  printf("%c\n", role);
  if (role == 'r')
    rw_readlock(&rwvar);

  return 0;
}
