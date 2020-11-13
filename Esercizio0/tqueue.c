#include "tqueue.h"

typedef struct TQueueNode {
    struct TQueueNode *next;
    void *data;
} TQueueNode;

unsigned long int tqueue_enqueue(TQueue *q, void *data) {
    if((*q)==NULL){
        (*q)=malloc(sizeof(struct TQueueNode));
        (*q)->data=data;
        (*q)->next=(*q);
        return 0;
    }
    TQueue head=*q;
    TQueue new_node=malloc(sizeof(struct TQueueNode));
    int i=0;
    while((*q)->next!=head){
        q=&(*q)->next;
        i++;
    }
    (*q)->next=new_node;
    new_node->next=head;
    new_node->data=data;
    return i+1;
}

void *tqueue_pop(TQueue *q) {
    if(*q==NULL)
        return NULL;
    TQueue *pointer=q;
    TQueue head=*q;
    *q=(*q)->next;
    while((*pointer)->next!=head)
        pointer=&(*pointer)->next;
    (*pointer)->next=head->next;
    return head;
}

unsigned long int tqueue_size(TQueue q) {
    if(q==NULL)return 0;
    int i=0;
    TQueue *pointer=&q;
    while((*pointer)->next!=q){
        pointer=&(*pointer)->next;
        i++;
    }
    return i+1;
}

TQueue tqueue_at_offset(TQueue q, unsigned long offset) {
    if(q==NULL)
        return NULL;
    int i=0;
    TQueue *indirect=&q;
    while(i!=offset) {
        indirect = &(*indirect)->next;
        i++;
    }
    return *indirect;
}

void *tqueue_get_data(TQueue q) {
    return q->data;
}

TQueue tqueue_get_next(TQueue q){
    return q->next;
}
