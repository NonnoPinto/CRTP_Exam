#pragma once
#include "BufferData.h"

int consumerCount[MAX_PROCESSES];
int producerCount = 0;

static void actor(struct BufferData *sharedBuf, int bufId, int nConsumers)
{
    int item, ID;
    struct msqid_ds info;
    bool fullArr = false;

    int users[nConsumers+1];
    for(int k = 0; k < nConsumers+1; k++)
        users[k] = -1;

    while (1)
    {
        sem_wait(&sharedBuf->actorSem); // 1->0

        msgctl(bufId, IPC_STAT, &info);
        //printf("->Actor, last consumer: %u\n", info.msg_lrpid);

        //printf("last user: %u\n", info.msg_lrpid);

        //Add id in "memory"
        for (int k = 1; k < nConsumers+1; k++){
            //if nobody has ever read the buffer
            if(info.msg_lrpid == 0)
                users[0] = info.msg_lspid;
            //if last user was producer
            if(info.msg_stime > info.msg_rtime){
                ID = 0;
                break;
            }
            //other cases
            if(!fullArr){
                //if more than one user but less than all users has read something
                if(users[k]==-1){
                    //printf("Assegnato il pid %d all'id %d\n", info.msg_lrpid, k);
                    users[k] = info.msg_lrpid;
                    ID = k;
                    //if im adding the last consumer
                    if(k == nConsumers){
                        fullArr = true;
                        //printf("Ok visti tutti\n");
                    }
                    break;
                }
                //if i get same ID before array is full
                else if (users[k] == info.msg_lrpid){
                    ID = k;
                    break;
                }
            }
            //search for existing ID when array is full
            else if (users[k] == info.msg_lrpid){
                        ID = k;
                        break;
            }    
        }

        //Output (for server, theorically)
        if(ID==0){
            printf("Last user was producer\n");
        }
        else{
            printf("Last user was consumer n. %d with pid %d\n", ID, info.msg_lrpid);
        }

        sem_post(&sharedBuf->mutexSem); // 0->1
        sleep(1);
    }
    printf("Per qualche ragione l'attore è uscito\n");
}