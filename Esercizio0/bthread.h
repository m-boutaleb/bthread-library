#ifndef ESERCIZIO0_BTHREAD_H
#define ESERCIZIO0_BTHREAD_H


/***************** STRUCTS *****************/
typedef struct {
} bthread_attr_t;

typedef void *(*bthread_routine)(void *);

typedef enum {
    __BTHREAD_READY = 0, __BTHREAD_BLOCKED, __BTHREAD_SLEEPING,
    __BTHREAD_ZOMBIE
} bthread_state;

typedef unsigned long int bthread_t;


/***************** METHODS *****************/
int bthread_create(bthread_t *bthread, const bthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);

int bthread_join(bthread_t bthread, void **retval);

void bthread_yield();

void bthread_exit(void *retval);

#endif //ESERCIZIO0_BTHREAD_H
