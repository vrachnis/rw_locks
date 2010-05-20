#ifndef RW_LOCK
#define RW_LOCK
struct rw_lock_t {
  int readers; /* readers that have locked the variable */
  int r_wait; /* readers waiting to lock the variable */
  int writers; /* writers that have locked the variable */
};

//pthread_mutex_t cmut = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t wmut = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t rmut = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t rcond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t tempmut = PTHREAD_MUTEX_INITIALIZER;

int rw_init(struct rw_lock_t **rw);
int rw_destroy(struct rw_lock_t **rw);
int rw_readlock(struct rw_lock_t **rw);
int rw_readunlock(struct rw_lock_t **rw);
int rw_writelock(struct rw_lock_t **rw);
int rw_writeunlock(struct rw_lock_t **rw);
#endif
