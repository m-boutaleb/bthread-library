#include <stdio.h>
#include "../../tqueue.h"
#include "../Unity-2.5.1/src/unity.h"

typedef unsigned long ul;

TQueue head;

void setUp(){
    head=NULL;
}

void tearDown(){
}

void test_pop(){
    // First add another element to the list
    ul data=668748L;
    tqueue_enqueue(&head,&data);

    TQueue toDelete=tqueue_pop(&head);
    free(toDelete);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1,tqueue_size(head), "Size should be 1");
}

void test_enqueue(){
    ul data=4685456L;

    ul value=tqueue_enqueue(&head, &data);
    ul result=*((ul *)tqueue_get_data(head));

    TEST_ASSERT_EQUAL_INT(data, result);
    TEST_ASSERT_EQUAL_INT(0, value);
}


int main(){
    test_enqueue();
    test_pop();
}
