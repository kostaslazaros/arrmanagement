//The start of the final project is here

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int makearr(int row, int col)
{
    int *arr = (int *)malloc(row * col * sizeof(int));
    int i, j;
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
            *(arr + i * col + j) = random() % 100;
    printf("The matrix is:\n");
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            printf("%d ", *(arr + i * col + j));
        }
        printf("\n");
    }
    free(arr);
    return 0;
}

int sem;
int sem_id = 1;
key_t key;

key = ftok("/virtualpathtosemaphore", 1);
// create a new semaphore
sem = semget(key, 1, IPC_CREAT);
// use sem = semget(key, 1, 0); to attach to an existing semaphore
// flags also contain access rights, to take care to set them appropriately

// increment semaphore
struct sembuf semopinc = {
    .sem_num = 0,
    .sem_op = 1,
    .sem_flg = 0};
semop(sem, &semopinc, 1);

/* decrement semaphore, may block */
struct sembuf semopdec = {
    .sem_num = 0,
    .sem_op = -1,
    .sem_flg = 0};
semop(sem, &semopdec, 1);

// sem_t mutex;
// void *thread(void *arg)
// {                     //function which act like thread
//     sem_wait(&mutex); //wait state
//     printf("\nEntered into the Critical Section..\n");
//     sleep(3);                   //critical section
//     printf("\nCompleted...\n"); //comming out from Critical section
//     sem_post(&mutex);
// }

//initializing shared memory safely
struct head
{
    unsigned volatile flag;
    pthread_mutex_t mut;
};

void *addr = 0;
/* try shm_open with exclusive, and then */
if (/* we create the segment */)
{
    addr = mmap(something);
    struct head *h = addr;
    pthread_mutex_init(&h->mut, aSharedAttr);
    pthread_mutex_lock(&h->mut);
    h->flag = 1;
    /* do the rest of the initialization, and then */
    pthread_mutex_unlock(&h->mut);
}
else
{
    /* retry shm_open without exclusive, and then */
    addr = mmap(something);
    struct head *h = addr;
    /* initialy flag is guaranteed to be 0 */
    /* this will break out of the loop whence the new value is written to flag */
    while (!h->flag)
        sched_yield();
    pthread_mutex_lock(&h->mut);
    pthread_mutex_unlock(&h->mut);
}

int main(int argc, char *argv[])
{
    makearray(4, 5);
    // sem_init(&mutex, 0, 1);
    // pthread_t th1, th2;
    // pthread_create(&th1, NULL, thread, NULL);
    // sleep(2);
    // pthread_create(&th2, NULL, thread, NULL);
    // //Join threads with the main thread
    // pthread_join(th1, NULL);
    // pthread_join(th2, NULL);
    // sem_destroy(&mutex);
    return 0;
}
