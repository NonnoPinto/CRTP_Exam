#pragma once
#include "BufferData.h"
//  roomAvailableSem=1
//  dataAvailableSem=0
//  mutexSem=1

static void producer(struct BufferData *sharedBuf, int bufId)
{
    int item = 15;
    struct msgbuf msg;
    msg.mtype = 1;
    while (1)
    {
        // producer
        sem_wait(&(sharedBuf->roomAvailableSem)); // 1->0
        sem_wait(&(sharedBuf->mutexSem));         // 1->0
        
        printf("-->Producer\n");

        //printf("producer: item: %d\n", item);
        
        //mem[item] = item;

        
        //sharedBuf->ID = -1;
        
        for(int i=0; i < 5; i++){
            msg.item = item;
            int res = msgsnd(bufId, &msg, sizeof(int), 0);
            /*Qui bisogna aggiungere l'errore!!*/
            sem_post(&sharedBuf->dataAvailableSem); // 0->N
            item++;
        }

        
        sem_post(&(sharedBuf->actorSem)); // 0->1
        sleep(1);
        //sem_post(&(sharedBuf->dataAvailableSem));
    }
}