#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/mman.h>

#include <semaphore.h>

#define BUFFER_SIZE 50
#define EMPTY 0
#define MAX_PROCESSES 10

//int mem[BUFFER_SIZE];

struct msgbuf
{
    long mtype;
    int item;
};

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

void getValues(struct BufferData buf){
    int m, d, r, a;
    sem_getvalue(&buf.mutexSem, &m);
    sem_getvalue(&buf.dataAvailableSem, &d);
    sem_getvalue(&buf.roomAvailableSem, &r);
    sem_getvalue(&buf.actorSem, &a);

    printf("mutex: %d\ndata: %d\nroom: %d\nactor: %d\n\n", m, d, r, a);
}