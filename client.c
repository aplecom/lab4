/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // создайте какую-нибудь работу, которую нужно выполнить
    struct data work;
    work.a = 5;
    work.b = 10;

    // инициализируем пул потоков
    pool_init();

    // отправить работу в очередь
    pool_submit(&add,&work);

    // может быть полезно
    //sleep(3);

    pool_shutdown();

    return 0;
}
