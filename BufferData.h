#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/socket.h>

#include <semaphore.h>
#include <pthread.h>

#include <netdb.h>
#include <netinet/in.h>

#define EMPTY 0
#define MAX_PROCESSES 10
#define BUFFER_SIZE 256

char* act;

struct msgbuf
{
    long mtype;
    int item;//[MAX_PROCESSES];
};

/* Buffer of semaphores */
struct BufferData
{
    // int readIdx;
    // int writeIdx;
    // int buffer[BUFFER_SIZE];
    // int ID;
    sem_t mutexSem;
    sem_t dataAvailableSem;
    sem_t roomAvailableSem;
    sem_t actorSem;
};


/* Helper function using during developement to know semaphores status */
void getValues(struct BufferData buf){
    int m, d, r, a;
    sem_getvalue(&buf.mutexSem, &m);            //only released by actor
    sem_getvalue(&buf.dataAvailableSem, &d);    //released by consumer after using items
    sem_getvalue(&buf.roomAvailableSem, &r);    //released after first items are created
    sem_getvalue(&buf.actorSem, &a);            //relased by producer or consumer

    printf("mutex: %d\ndata: %d\nroom: %d\nactor: %d\n\n", m, d, r, a);
}