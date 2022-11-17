#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *pthread_run1(void *arg){
    while (1)
    {
        printf("I am thread1,ID: %x\n",pthread_self());
        sleep(1);
    }
}

void *pthread_run2(void *arg){
    while (1)
    {
        printf("I am thread2,ID: %x\n",pthread_self());
        sleep(1);
    }
}
int main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1,NULL,pthread_run1,NULL);
    pthread_create(&thread2,NULL,pthread_run2,NULL);
    printf("I am main thread.\n");
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    return 0 ;
}
