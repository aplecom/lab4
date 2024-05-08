/**
 * Реализация пула потоков.
 */
#include "list.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define NUMBER_OF_THREADS 3


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
int enqueue(Task task)
{
    int i = 0;

    Task *newTask = malloc(sizeof(Task));
    if (!newTask) {
        i = 1;
        return i;
    }

    newTask->function = task.function;
    newTask->data = task.data;

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
    int i = 0;

    worktodo.function = somefunction;
    worktodo.data = p;
    i = enqueue(worktodo);

    if (!i)
        sem_post(&sem); // Это сигнализирует потокам в пуле, что появилась новая задача для выполнения.

    return i;
}

// инициализируйте пул потоков
void pool_init(void)
{
    // 1 аргумент сам мьютекс , а второй
    // NULL, или указатель на объект pthread_mutexattr_t, который указывает атрибуты,
    // которые требуется использовать для мьютекса.
    pthread_mutex_init(&queue_lock, NULL);
    //  2 аргумент - Если равен 0, то семаформ - общий
    //  для потоков, если ненулевое, то  для процессов
    // 3 арг - начальное значение семафора
    sem_init(&sem, 0, 0);
    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_create(&bees[i],NULL,worker,NULL);
    }
}

// завершите работу пула потоков
void pool_shutdown(void)
{
    pthread_mutex_destroy(&queue_lock);
    sem_destroy(&sem);
    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        // прерывания потока
        pthread_cancel(bees[i]);
        // ожидания завершения потока - блокирует вызывающий поток, пока указанный поток не завершится
        pthread_join(bees[i],NULL);
        // 2 арг -   принимает значение статуса выхода завершенного потока при успешном завершении
    }
}
