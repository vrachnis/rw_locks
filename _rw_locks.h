#ifndef RW_LOCK
#define RW_LOCK
#include <pthread.h>

struct rw_lock_t {
  int readers; /* readers that have locked the variable */
  int writers; /* writers that have locked the variable */
  pthread_mutex_t WriterConditionMutex = PTHREAD_MUTEX_INITIALIZER; /* A mutex for the condition the writers use */
  pthread_mutex_t WriterMutex = PTHREAD_MUTEX_INITIALIZER;		/* A mutex that ensures that only one writer is actually writing at each point */
  pthread_cond_t WriterCondition = PTHREAD_COND_INITIALIZER;	/* The condition that wakes all the readers */
  pthread_mutex_t ReaderConditionMutex = PTHREAD_MUTEX_INITIALIZER; /* A mutex for the condition the readers use */
  pthread_cond_t ReaderCondition = PTHREAD_COND_INITIALIZER;	/* The condition that wakes all the writers */
};

int rw_init(struct rw_lock_t **rw);
int rw_destroy(struct rw_lock_t **rw);
int rw_readlock(struct rw_lock_t **rw);
int rw_readunlock(struct rw_lock_t **rw);
int rw_writelock(struct rw_lock_t **rw);
int rw_writeunlock(struct rw_lock_t **rw);
#endif
