#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "imageIO.h"
#include "PQueue.h"

PQueue* newPQueue(int data, int priority){
    PQueue* temp = (PQueue*)malloc(sizeof(PQueue));
    temp->data = data;
    temp->priority = priority;
    temp->next = NULL;

    return temp;

};
int peek(PQueue** head){
    return (*head)->data;

};
void pop(PQueue** head){
    PQueue* temp = *head;
    (*head) = (*head)->next;
    free(temp);
};

void push(PQueue** head, int d, int p){
    PQueue* start = (*head);

    // Create new Node
    PQueue* temp = newPQueue(d, p);

    // Special Case: The head of list has lesser
    // priority than new node. So insert new
    // node before head node and change head node.
    if ((*head)->priority > p) {

        // Insert New Node before head
        temp->next = *head;
        (*head) = temp;
    }
    else {

        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL &&
            start->next->priority < p) {
            start = start->next;
        }

        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }

};
int isEmpty(PQueue** head){

    return (*head) == NULL;

};
