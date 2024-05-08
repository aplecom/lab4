/**
 * Реализация пула потоков.
 */
#include "list.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// это представляет собой работу, которая должна быть
// завершена потоком в пуле


// рабочая очередь
Task worktodo;


// рабочие  bee ( id потокка )
pthread_t bees[NUMBER_OF_THREADS];
// мьютекс инициализируется в свое стандартное состояние, где он не заблокирован и не имеет атрибутов
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

struct node *queue = NULL;
// вставьте задачу в очередь
// возвращает 0 в случае успеха или 1 в противном случае,
int enqueue(Task t)
{
    int i = 0;

    Task *newTask = malloc(sizeof(Task));
    if (!newTask) {
        i = 1;
        return i;
    }

    newTask->function = t.function;
    newTask->data = t.data;

    pthread_mutex_lock(&queue_lock);
    insert(&queue, newTask);
    pthread_mutex_unlock(&queue_lock);

    return i;
}

// удаление задачи из очереди для получения работы из очереди
Task dequeue()
{
    pthread_mutex_lock(&queue_lock);
    delete(&queue, queue->task);
    pthread_mutex_unlock(&queue_lock);
    return worktodo;
}
// рабочий поток в пуле потоков
void *worker(void *param)
{
    // выполните задание
    while (queue != NULL) {
        sem_wait(&sem);
        execute(queue->task->function, queue->task->data);
        dequeue();
    }
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
