#include <setjmp.h>
#include <stdio.h>

#define CUSHION_SIZE 10000
#define save_context(CONTEXT) setjmp(CONTEXT)
#define restore_context(CONTEXT) longjmp(CONTEXT, 1)

typedef enum {
    __BTHREAD_UNINITIALIZED, __BTHREAD_READY, __BTHREAD_DONE
} bthread_state;

typedef void *(*bthread_routine)(void *);

void create_cushion_and_call(bthread_routine fn, bthread_state *state);

void *bthread1(void *arg);

void *bthread2(void *arg);

void *bthread3(void *arg);

void *bthread4(void *arg);

jmp_buf bthread1_buf, bthread2_buf, bthread3_buf, bthread4_buf;
jmp_buf *all_threads[4];
//bthread_state bthread1_state = __BTHREAD_UNINITIALIZED;
//bthread_state bthread2_state = __BTHREAD_UNINITIALIZED;
//bthread_state bthread3_state = __BTHREAD_UNINITIALIZED;
//bthread_state bthread4_state = __BTHREAD_UNINITIALIZED;
bthread_state all_threads_states[4];

void print(const int thread_nr) {
    static int is_printed[]={0,0,0,0};
    if(!is_printed[thread_nr-1]) {
        printf("##################### Thread %d finished #####################\n", thread_nr);
        is_printed[thread_nr - 1] = 1;
    }
}

void join() {
    for (int i = 0; i < 4; i++)
        if (all_threads_states[i] != __BTHREAD_DONE)
            restore_context(*all_threads[i]);
}

void create_cushion_and_call(bthread_routine fn, bthread_state *state) {
    char cushion[CUSHION_SIZE];
    cushion[CUSHION_SIZE - 1] = 1;
    *state = __BTHREAD_READY;
    fn(NULL);
}

void *bthread1(void *arg) {
    volatile int i;
    for (i = 0; i < 10000; i++) {
        printf("BThread1, i=%d\n", i);
        //Prioritize the current thread to make it execute primarly
        if (i % 1000 == 0)
            /* Yield to next bthread */
            if (!save_context(*(all_threads[0]))) {
                if (all_threads_states[1] == __BTHREAD_UNINITIALIZED) {
                    create_cushion_and_call(bthread2, &all_threads_states[1]);
                } else {
                    restore_context(*all_threads[1]);
                }
            }
    }
//    bthread1_state=__BTHREAD_DONE;
    all_threads_states[0] = __BTHREAD_DONE;
    print(1);

    join();
}

void *bthread2(void *arg) {
    volatile int j;
    for (j = 0; j < 10000; j++) {
        printf("BThread2, i=%d\n", j);
        /* Yield to next bthread */
        if (!save_context(*all_threads[1])) {
            if (all_threads_states[2] == __BTHREAD_UNINITIALIZED) {
                create_cushion_and_call(bthread3, &all_threads_states[2]);
            } else {
                restore_context(*all_threads[2]);
            }
        }
    }
//    bthread2_state=__BTHREAD_DONE;
    all_threads_states[1] = __BTHREAD_DONE;
    print(2);
    join();
}

void *bthread3(void *arg) {
    volatile int z;
    for (z = 0; z < 10000; z++) {
        printf("BThread3, i=%d\n", z);
        // Yield to next bthread
        // We assume that bthread1 is already initialized
        if (!save_context(*all_threads[2])) {
            if (all_threads_states[3] == __BTHREAD_UNINITIALIZED) {
                create_cushion_and_call(bthread4, &all_threads_states[3]);
            } else {
                restore_context(*all_threads[3]);
            }
        }
    }
    all_threads_states[2] = __BTHREAD_DONE;
    print(3);
//    bthread3_state=__BTHREAD_DONE;
    join();
}

void *bthread4(void *arg) {
    volatile int z;
    for (z = 0; z < 10000; z++) {
        printf("BThread4, i=%d\n", z);
        // Yield to next bthread
        // We assume that bthread1 is already initialized
        if (!save_context(*all_threads[3]))
            restore_context(*all_threads[0]);
    }
    //bthread4_state=__BTHREAD_DONE;
    all_threads_states[3] = __BTHREAD_DONE;
    print(4);
    join();
}

int main() {
    all_threads[0] = &bthread1_buf;
    all_threads[1] = &bthread2_buf;
    all_threads[2] = &bthread3_buf;
    all_threads[3] = &bthread4_buf;
    create_cushion_and_call(bthread1, &all_threads_states[0]);
    return 0;
}