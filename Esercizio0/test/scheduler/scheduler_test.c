#include <stdio.h>
#include <unistd.h>
#include "../../bthread.h"
//#include "../Unity-2.5.1/src/unity.h"


void *routine(void *args) {
//    printf("Hello I'm %s", (char *) args);
    return (void *) 45;
}

int main() {
    int i, j;
    bthread_t thread, thread2;
    bthread_create(&thread, NULL, &routine, "Mohamed");
    bthread_create(&thread2, NULL, &routine, "Mohamed");
    sleep(1);
    bthread_join(thread, (void **) &i);
    bthread_join(thread2, (void **) &j);
    printf("\nReturn value is %d", i);
    printf("\nReturn value is %d", j);

    //TEST_ASSERT_EQUAL_INT_MESSAGE(45, i, "Value should be 45");
    return 0;
}