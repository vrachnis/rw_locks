#ifndef RW_LOCK
#define RW_LOCK
struct rw_lock_t {
  int readers; /* readers that have locked the variable */
  int writers; /* writers that have locked the variable */
};

int rw_init(struct rw_lock_t *rw);
int rw_destroy(struct rw_lock_t **rw);
int rw_readlock(struct rw_lock_t **rw);
int rw_readunlock(struct rw_lock_t **rw);
int rw_writelock(struct rw_lock_t **rw);
int rw_writeunlock(struct rw_lock_t **rw);
#endif
