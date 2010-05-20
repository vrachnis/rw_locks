#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "_rw_locks.h"

/* extern pthread_mutex_t cmut; */
/* extern pthread_mutex_t wmut; */
/* extern pthread_cond_t cond; */
/* extern pthread_mutex_t rmut; */
/* extern pthread_cond_t rcond; */
/* extern pthread_cond_t tempmut; */

int 
rw_init(struct rw_lock_t **rw)
{
  if ((*rw = (struct rw_lock_t *) malloc(sizeof(struct rw_lock_t))) == NULL) {
    perror("Malloc failed.");
    exit(1);
  }

  (*rw)->readers = 0;
  (*rw)->r_wait = 0;
  (*rw)->writers = 0;

  if ((*rw)->readers == 0 && (*rw)->r_wait == 0 && (*rw)->writers == 0)
    return 0;
  else
    return 1;
}

int
rw_destroy(struct rw_lock_t **rw)
{
  free(*rw);
  pthread_cond_destroy(&cond);
  pthread_cond_destroy(&rcond);
  pthread_mutex_destroy(&cmut);
  pthread_mutex_destroy(&wmut);
  pthread_mutex_destroy(&rmut);
  pthread_mutex_destroy(&tempmut);

  return 0;
}

int
rw_readlock(struct rw_lock_t **rw)
{
  if ((*rw)->readers < 0) {
    printf("READER PANIC: %d\n", (*rw)->readers);
    exit(2);
  }
  pthread_mutex_lock(&tempmut);
  (*rw)->r_wait++;
  pthread_mutex_unlock(&tempmut);
  
  //printf("readers: %d\n", (*rw)->readers);
  
  pthread_mutex_lock(&cmut);
  while((*rw)->writers > 0) {
    pthread_cond_wait(&cond, &cmut);
  }
  pthread_mutex_unlock(&cmut);

  pthread_mutex_lock(&rmut);
  pthread_mutex_lock(&tempmut);
  (*rw)->r_wait--;
  (*rw)->readers++;
  pthread_mutex_unlock(&tempmut);
  pthread_mutex_unlock(&rmut);

  return 0;
}

int
rw_readunlock(struct rw_lock_t **rw)
{
  //(*rw)->readers--;
  /* if ((*rw)->readers < 0) { */
  /*   printf("READER PANIC\n"); */
  /*   exit(2); */
  /* } */

  /* printf("waiting readers: %d\n", (*rw)->r_wait); */
  /* printf("readers: %d\n", (*rw)->readers); */

  pthread_mutex_lock(&rmut);
  pthread_mutex_lock(&tempmut);
  (*rw)->readers--;
  pthread_mutex_unlock(&tempmut);
  if ((*rw)->readers == 0) {
    //printf("waking writers...\n");
    pthread_cond_broadcast(&rcond);
  }
  pthread_mutex_unlock(&rmut);

  return 0;
}

int
rw_writelock(struct rw_lock_t **rw)
{
  pthread_mutex_lock(&cmut);
  (*rw)->writers++;

  if ((*rw)->writers > 2) {
    printf("WRITER PANIC\n");
    exit(2);
  }
  pthread_mutex_unlock(&cmut);
  
  //printf("writers: %d\n", (*rw)->writers);

  pthread_mutex_lock(&rmut);
  while((*rw)->readers > 0) {
    pthread_cond_wait(&rcond, &rmut);
  }
  pthread_mutex_unlock(&rmut);

  pthread_mutex_lock(&wmut);

  return 0;
}

int
rw_writeunlock(struct rw_lock_t **rw)
{
  pthread_mutex_lock(&cmut);
  (*rw)->writers--;
  pthread_mutex_unlock(&cmut);
  
  //printf("writers: %d\n", (*rw)->writers);

  pthread_mutex_lock(&cmut);
  //(*rw)->writers--;
  if ((*rw)->writers == 0) {
    //printf("waking readers\n");
    pthread_cond_broadcast(&cond);
  }
  pthread_mutex_unlock(&cmut);

  pthread_mutex_unlock(&wmut);

  return 0;
}
