#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "_rw_locks.h"

#define NR_THREADS 16
#define TERM_NR 10000

void *init_function(void *ptr);
int inc();
int counter = 0;

struct rw_lock_t *rwvar=NULL;

int main(int argc, char **argv)
{
  if (rw_init(&rwvar)) {
    perror("Initialization failed");
    exit(1);
  }

  printf("readers waiting: %d\nreaders: %d\nwriters: %d\n", rwvar->r_wait, rwvar->readers, rwvar->writers);

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

  printf("readers waiting: %d\nreaders: %d\nwriters: %d\n", rwvar->r_wait, rwvar->readers, rwvar->writers);
  /* printf("Thread 1 returns: %d\n", ret1); */
  /* printf("Thread 2 returns: %d\n", ret2); */

  if (rw_destroy(&rwvar)) {
    perror("Destruction failed");
    exit(1);
  }

  return 0;
}

void
*init_function(void *ptr)
{
  /* char *message=NULL; */
  /* message = (char *) ptr; */
  /* printf("%s Initialized.\n", message); */
  char role;
  role = (char) ptr;
  printf("%c\n", role);
  if (role == 'r') {
    int mine;
    for (;;) {
      rw_readlock(&rwvar);
      mine = counter;
      //printf("%d ", mine);
      rw_readunlock(&rwvar);
      if (mine == TERM_NR) {
	printf("reader done %d:%d:%d\n",rwvar->r_wait,rwvar->readers,rwvar->writers);
	pthread_exit(NULL);
      }
    }
  }
  else {
    int ret;
    for (;;) {
      rw_writelock(&rwvar);
      ret = inc();
      rw_writeunlock(&rwvar);
      if (ret) {
	pthread_exit(NULL);
      }
    }
  }
  
}

int
inc()
{
  if (counter < TERM_NR) {
    printf("+");
    counter++;
    return 0;
  }
  else {
    printf("\n%d i'm done\n", rwvar->writers);
    return 1;
  }
}
