#include "BufferData.h"

#include "client.c"
#include "producer.c"
#include "consumer.c"

int main(int argc, char *args[])
{

    act = malloc(BUFFER_SIZE*sizeof(char));

    //Initializa shared buffer
    //Special thanks to https://community.hpe.com/t5/hp-ux-general/sem-wait-and-forked-processes/td-p/2783079 for this snippet
    struct BufferData *sharedBuf;
    void *shmemblk;
    shmemblk = mmap(NULL, sizeof(sharedBuf), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sharedBuf = (struct BufferData*)shmemblk;

    //minor checks
    int i, nConsumers;
    pid_t pids[MAX_PROCESSES];

    if (argc != 2)
    {
        printf("Cannot have less than 2 consumers\n");
        exit(0);
    }

    sscanf(args[1], "%d", &nConsumers);

    //Initialize message queue
    int bufId;
    bufId = msgget(IPC_PRIVATE, 0666);
    //printf("Il buffer è andato %d\n", bufId);
    //error
    if (bufId == -1){
        perror("msgget");
        exit(0);
    }

    //Semaphores init
    sem_init(&sharedBuf->mutexSem, 1, 1);
    sem_init(&sharedBuf->dataAvailableSem, 1, 0);
    sem_init(&sharedBuf->roomAvailableSem, 1, 1);
    sem_init(&sharedBuf->actorSem, 1, 0);

    /* Launch producer process */
    pids[0] = fork();
    if (pids[0] == 0)
    {
        /* Child process */
        producer(sharedBuf, bufId);
        exit(0);
        printf("Eliminato producer\n");
    }
    /* Launch actor process */
    pids[1] = fork();
    if (pids[1] == 0)
    {
        /* Child process */
        actor(sharedBuf, bufId, nConsumers);
        exit(0);
        printf("Eliminato attore\n");
    }
    /* Launch consumer processes */
    for (i = 0; i < nConsumers && i < MAX_PROCESSES - 1; i++)
    {
        pids[i + 2] = fork();
        if (pids[i + 2] == 0)
        {
            //consumer
            consumer(sharedBuf, i, bufId);
            exit(0);
            printf("Distrutto figlio %d\n", i);
        }
    }
    /* Wait process termination, mostly will not be reached */
    for (i = 0; i <= nConsumers; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}