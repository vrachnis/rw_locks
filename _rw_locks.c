#include <stdio.h>
#include <stdlib.h>
/* #include <pthread.h> */
#include "_rw_locks.h"

int 
rw_init(struct rw_lock_t **rw)
{
  if ((*rw = (struct rw_lock_t *) malloc(sizeof(struct rw_lock_t))) == NULL) {
    perror("Malloc failed.");
    exit(1);
  }

  (*rw)->readers = 0;
  (*rw)->writers = 0;
  /* (*rw)->WriterConditionMutex = PTHREAD_MUTEX_INITIALIZER; */
  /* (*rw)->WriterMutex = PTHREAD_MUTEX_INITIALIZER; */
  /* (*rw)->WriterCondition = PTHREAD_COND_INITIALIZER; */
  /* (*rw)->ReaderConditionMutex = PTHREAD_MUTEX_INITIALIZER; */
  /* (*rw)->ReaderCondition = PTHREAD_COND_INITIALIZER; */

  if ((*rw)->readers == 0 && (*rw)->writers == 0)
    return 0;
  else
    return 1;
}

int
rw_destroy(struct rw_lock_t **rw)
{
  pthread_cond_destroy(&(*rw)->WriterCondition);
  pthread_cond_destroy(&(*rw)->ReaderCondition);
  pthread_mutex_destroy(&(*rw)->WriterConditionMutex);
  pthread_mutex_destroy(&(*rw)->WriterMutex);
  pthread_mutex_destroy(&(*rw)->ReaderConditionMutex);
  free(*rw);

  return 0;
}

int
rw_readlock(struct rw_lock_t **rw)
{
  if ((*rw)->readers < 0) {
    printf("READER PANIC: %d\n", (*rw)->readers);
    exit(2);
  }
  
  pthread_mutex_lock(&(*rw)->WriterConditionMutex);
  while((*rw)->writers > 0) {
    pthread_cond_wait(&(*rw)->WriterCondition, &(*rw)->WriterConditionMutex);
  }
  pthread_mutex_unlock(&(*rw)->WriterConditionMutex);

  pthread_mutex_lock(&(*rw)->ReaderConditionMutex);
  (*rw)->readers++;
  pthread_mutex_unlock(&(*rw)->ReaderConditionMutex);

  return 0;
}

int
rw_readunlock(struct rw_lock_t **rw)
{
  if ((*rw)->readers < 0) {
    printf("READER PANIC\n");
    exit(2);
  }

  pthread_mutex_lock(&(*rw)->ReaderConditionMutex);
  (*rw)->readers--;
  if ((*rw)->readers == 0) {
    pthread_cond_broadcast(&(*rw)->ReaderCondition);
  }
  pthread_mutex_unlock(&(*rw)->ReaderConditionMutex);

  return 0;
}

int
rw_writelock(struct rw_lock_t **rw)
{
  pthread_mutex_lock(&(*rw)->WriterConditionMutex);
  (*rw)->writers++;

  if ((*rw)->writers > 2) {
    printf("WRITER PANIC\n");
    exit(2);
  }
  pthread_mutex_unlock(&(*rw)->WriterConditionMutex);
  
  pthread_mutex_lock(&(*rw)->ReaderConditionMutex);
  while((*rw)->readers > 0) {
    pthread_cond_wait(&(*rw)->ReaderCondition, &(*rw)->ReaderConditionMutex);
  }
  pthread_mutex_unlock(&(*rw)->ReaderConditionMutex);

  pthread_mutex_lock(&(*rw)->WriterMutex);

  return 0;
}

int
rw_writeunlock(struct rw_lock_t **rw)
{
  pthread_mutex_lock(&(*rw)->WriterConditionMutex);
  (*rw)->writers--;
  pthread_mutex_unlock(&(*rw)->WriterConditionMutex);
  
  pthread_mutex_lock(&(*rw)->WriterConditionMutex);
  if ((*rw)->writers == 0) {
    pthread_cond_broadcast(&(*rw)->WriterCondition);
  }
  pthread_mutex_unlock(&(*rw)->WriterConditionMutex);

  pthread_mutex_unlock(&(*rw)->WriterMutex);

  return 0;
}
