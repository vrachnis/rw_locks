struct rw_lock_t {
  int readers; /* readers that have locked the variable */
  int writers; /* writers that have locked the variable */
};

int 
rw_init(struct rw_lock_t *rw)
{
  if ((rw = (struct rw_lock_t *) malloc(sizeof(struct rw_lock_t))) == NULL) {
    perror("Malloc failed.");
    exit(1);
  }

  rw->readers = 0;
  rw->writers = 0;

  if (rw->readers == 0 && rw->writers == 0)
    return 0;
  else
    return 1;
}

int
rw_destroy(struct rw_lock_t *rw)
{
  free(rw);

  return 0;
}
