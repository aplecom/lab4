/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task worktodo;

// the worker bee
pthread_t bee;

// вставьте задачу в очередь
// возвращает 0 в случае успеха или 1 в противном случае,
int enqueue(task t) 
{
    return 0;
}

// удаление задачи из очереди
task dequeue() 
{
    return worktodo;
}

// рабочий поток в пуле потоков
void *worker(void *param)
{
    // execute the task
    execute(worktodo.function, worktodo.data);

    pthread_exit(0);
}

/**
 * Выполняет задачу, предоставленную пулу потоков
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Отправляет работу в пул.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    return 0;
}

// инициализируйте пул потоков
void pool_init(void)
{
    pthread_create(&bee,NULL,worker,NULL);
}

// завершите работу пула потоков
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
}
