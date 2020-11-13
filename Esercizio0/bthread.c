#include <stdint.h>
#include <stdio.h>
#include "bthread_private.h"

/***************** DEFINE *****************/
#define save_context(CONTEXT) sigsetjmp(CONTEXT, 1)
#define restore_context(CONTEXT) siglongjmp(CONTEXT, 1)
#define STACK_SIZE 60000

/***************** GLOBAL VARS *****************/
static __bthread_scheduler_private *instance = NULL;
static int idCounter = 0;

/***************** PRIVATE *****************/
static __bthread_scheduler_private *bthread_get_scheduler() {
    if (instance == NULL) {
        instance = (__bthread_scheduler_private *) malloc(sizeof(__bthread_scheduler_private));
        instance->queue = NULL;
    }

    return instance;
}

static TQueue bthread_get_queue_at(bthread_t bthread) {
    TQueue head = bthread_get_scheduler()->queue;
    while (tqueue_get_next(head)) {
        if (((__bthread_private *) tqueue_get_data(head))->tid == bthread)break;
        head = tqueue_at_offset(head,1);
    }
    return head;
}

static int bthread_check_if_zombie(bthread_t bthread, void **retval) {
    TQueue thread = bthread_get_queue_at(bthread);
//    if (!thread)printf("Something gone wrong");
    __bthread_private *found = (__bthread_private *) tqueue_get_data(thread);

    if (found->tid == bthread && found->state != __BTHREAD_ZOMBIE)
        return 0;

    if (retval)*retval = (found->retval);

    free(found->stack);
    TQueue theNext=tqueue_get_next(thread);

    if (tqueue_get_data(thread) == tqueue_get_data(bthread_get_scheduler()->queue))
        free(tqueue_pop(&bthread_get_scheduler()->queue));
    else
        free(tqueue_pop(&thread));

    bthread_get_scheduler()->queue=theNext;

    return 1;
}

static void bthread_cleanup() {
}


/***************** PUBLIC IMPLEMENTATIONS *****************/
int bthread_create(bthread_t *bthread, const bthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    __bthread_private *newThread = malloc(sizeof(__bthread_private));
    TQueue *head = &bthread_get_scheduler()->queue;
    newThread->tid = idCounter;
    *bthread = idCounter++;
    newThread->attr = *attr;
    newThread->body = start_routine;
    newThread->arg = arg;
    newThread->stack = NULL;
    newThread->state = __BTHREAD_READY;
    tqueue_enqueue(head, newThread);
}

int bthread_join(bthread_t bthread, void **retval) {
    volatile __bthread_scheduler_private *scheduler = bthread_get_scheduler();
    scheduler->current_item = scheduler->queue;
    save_context(scheduler->context);
    if (bthread_check_if_zombie(bthread, retval)) {
        return 0;
    }
    __bthread_private *tp;
    do {
        scheduler->current_item = tqueue_at_offset(scheduler->current_item, 1);
        tp = (__bthread_private *) tqueue_get_data(scheduler->current_item);
    } while (tp->state != __BTHREAD_READY);
    if (tp->stack) {
        restore_context(tp->context);
    } else {
        tp->stack = (char *) malloc(sizeof(char) * STACK_SIZE);
        unsigned long target = tp->stack + STACK_SIZE - 1;
#if __APPLE__
        // OSX requires 16 bytes aligned stack
        target -= (target % 16);
#endif
#if __x86_64__
        asm __volatile__("movq %0, %%rsp" :: "r"((intptr_t) target));
#else
        asm __volatile__("movl %0, %%esp"::"r"((intptr_t) target));
#endif
//        printf("Ciarrivo");
        bthread_exit(tp->body(tp->arg));
    }
    return 0;
}

void bthread_yield() {
    if (!save_context(((__bthread_private *) tqueue_get_data(bthread_get_scheduler()->current_item))->context))
        restore_context(bthread_get_scheduler()->context);
}

void bthread_exit(void *retval) {
    __bthread_private *current = ((__bthread_private *) tqueue_get_data(bthread_get_scheduler()->current_item));
    current->state = __BTHREAD_ZOMBIE;
    current->retval = retval;
    bthread_yield();
}

