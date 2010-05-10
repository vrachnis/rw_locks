struct rw_lock_t {
  int readers; /* readers that have locked the variable */
  int r_wait; /* readers waiting to lock the variable */
  int writers; /* writers that have locked the variable */
};

extern pthread_mutex_t cmut;
extern pthread_mutex_t wmut;
extern pthread_cond_t cond;
extern pthread_mutex_t rmut;
extern pthread_cond_t rcond;


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

  return 0;
}

int
rw_readlock(struct rw_lock_t **rw)
{
  printf("locking for reader\n");
  (*rw)->r_wait++;
  
  printf("readers: %d\n", (*rw)->readers);
  
  pthread_mutex_lock(&cmut);
  while((*rw)->writers >0) {
    pthread_cond_wait(&cond, &cmut);
  }
  (*rw)->r_wait--;
  (*rw)->readers++;

  pthread_mutex_unlock(&cmut);

  return 0;
}

int
rw_readunlock(struct rw_lock_t **rw)
{
  printf("unlocking for reader\n");
  (*rw)->readers--;

  printf("readers: %d\n", (*rw)->readers);

  pthread_mutex_lock(&rmut);
  if ((*rw)->writers) {
    pthread_cond_signal(&rcond);
  }
  pthread_mutex_unlock(&rmut);
}

int
rw_writelock(struct rw_lock_t **rw)
{
  printf("locking for writer\n");
  (*rw)->writers++;
  
  printf("writers: %d\n", (*rw)->writers);

  pthread_mutex_lock(&rmut);
  while((*rw)->readers >0) {
    pthread_cond_wait(&rcond, &rmut);
  }
  pthread_mutex_unlock(&rmut);

  pthread_mutex_lock(&wmut);

  return 0;
}

int rw_writeunlock(struct rw_lock_t **rw)
{
  printf("unlocking for writer\n");
  (*rw)->writers--;
  
  printf("writers: %d\n", (*rw)->writers);

  pthread_mutex_lock(&cmut);
  if (!(*rw)->writers) {
    pthread_cond_signal(&cond);
  }
  pthread_mutex_unlock(&cmut);

  pthread_mutex_unlock(&wmut);

  return 0;
}
