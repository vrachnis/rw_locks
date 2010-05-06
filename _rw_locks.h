struct rw_lock_t {
  int readers; /* readers that have locked the variable */
  int writers; /* writers that have locked the variable */
};

extern pthread_mutex_t cmut;
extern pthread_mutex_t wmut;
extern pthread_cond_t cond;


int 
rw_init(struct rw_lock_t **rw)
{
  if ((*rw = (struct rw_lock_t *) malloc(sizeof(struct rw_lock_t))) == NULL) {
    perror("Malloc failed.");
    exit(1);
  }

  (*rw)->readers = 0;
  (*rw)->writers = 0;

  if ((*rw)->readers == 0 && (*rw)->writers == 0)
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
  (*rw)->readers++;
  
  printf("readers: %d\n", (*rw)->readers);
  
  pthread_mutex_lock(&cmut);
  while((*rw)->writers >0) {
    pthread_cond_wait(&cond, &cmut);
  }
  pthread_mutex_unlock(&cmut);

  return 0;    
}

int
rw_writelock(struct rw_lock_t **rw)
{
  (*rw)->writers++;
  
  pthread_mutex_lock(&cmut);
  if (!(*rw)->writers) {
    pthread_cond_signal(&cond);
  }
  pthread_mutex_unlock(&cmut);

  return 0;
}

