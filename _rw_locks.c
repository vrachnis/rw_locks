#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "_rw_locks.h"

pthread_mutex_t WriteReaderConditionitionMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t WriteReaderConditionMutexex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t WriteReaderConditionition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ReaderConditionMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ReaderCondition = PTHREAD_COND_INITIALIZER;

int 
rw_init(struct rw_lock_t *rw)
{
  struct rw_lock_t *temp;

  if ((temp = (struct rw_lock_t*) malloc(sizeof(struct rw_lock_t))) == NULL) {
    perror("Malloc failed.");
    exit(1);
  }

  temp->readers = 0;
  temp->writers = 0;

  rw=temp;

  if (rw->readers == 0 && rw->writers == 0)
    return 0;
  else
    return 1;
}

int
rw_destroy(struct rw_lock_t **rw)
{
  free(*rw);
  pthread_cond_destroy(&WriteReaderConditionition);
  pthread_cond_destroy(&ReaderCondition);
  pthread_mutex_destroy(&WriteReaderConditionitionMutex);
  pthread_mutex_destroy(&WriteReaderConditionMutexex);
  pthread_mutex_destroy(&ReaderConditionMutex);

  return 0;
}

int
rw_readlock(struct rw_lock_t **rw)
{
  if ((*rw)->readers < 0) {
    printf("READER PANIC: %d\n", (*rw)->readers);
    exit(2);
  }
  
  pthread_mutex_lock(&WriteReaderConditionitionMutex);
  while((*rw)->writers > 0) {
    pthread_cond_wait(&WriteReaderConditionition, &WriteReaderConditionitionMutex);
  }
  pthread_mutex_unlock(&WriteReaderConditionitionMutex);

  pthread_mutex_lock(&ReaderConditionMutex);
  (*rw)->readers++;
  pthread_mutex_unlock(&ReaderConditionMutex);

  return 0;
}

int
rw_readunlock(struct rw_lock_t **rw)
{
  if ((*rw)->readers < 0) {
    printf("READER PANIC\n");
    exit(2);
  }

  pthread_mutex_lock(&ReaderConditionMutex);
  (*rw)->readers--;
  if ((*rw)->readers == 0) {
    pthread_cond_broadcast(&ReaderCondition);
  }
  pthread_mutex_unlock(&ReaderConditionMutex);

  return 0;
}

int
rw_writelock(struct rw_lock_t **rw)
{
  pthread_mutex_lock(&WriteReaderConditionitionMutex);
  (*rw)->writers++;

  if ((*rw)->writers > 2) {
    printf("WRITER PANIC\n");
    exit(2);
  }
  pthread_mutex_unlock(&WriteReaderConditionitionMutex);
  
  pthread_mutex_lock(&ReaderConditionMutex);
  while((*rw)->readers > 0) {
    pthread_cond_wait(&ReaderCondition, &ReaderConditionMutex);
  }
  pthread_mutex_unlock(&ReaderConditionMutex);

  pthread_mutex_lock(&WriteReaderConditionMutexex);

  return 0;
}

int
rw_writeunlock(struct rw_lock_t **rw)
{
  pthread_mutex_lock(&WriteReaderConditionitionMutex);
  (*rw)->writers--;
  pthread_mutex_unlock(&WriteReaderConditionitionMutex);
  
  pthread_mutex_lock(&WriteReaderConditionitionMutex);
  if ((*rw)->writers == 0) {
    pthread_cond_broadcast(&WriteReaderConditionition);
  }
  pthread_mutex_unlock(&WriteReaderConditionitionMutex);

  pthread_mutex_unlock(&WriteReaderConditionMutexex);

  return 0;
}
