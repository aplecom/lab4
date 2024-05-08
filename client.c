/**
 * Пример клиентской программы, использующей пул потоков.
 */
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

int get_random(int max) {
    return random() % max;
}

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    srand(time(NULL));
    // создайте какую-нибудь работу, которую нужно выполнить
    struct data work[10];
    for (int i = 0; i < 10; ++i) {
        work[i].a = get_random(111);
        work[i].b = get_random(222);
    }

    // инициализируем пул потоков
    pool_init();

    // отправить работу в очередь
    pool_submit(&add,&work);
    for (int i = 0; i < 10; ++i) {
        pool_submit(&add,&work[i]);
    }
    pool_shutdown();

    return 0;
}
