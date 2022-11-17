#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *son_thread_func(void *arg){
    int j = 0 ;
    while (1){
        j++ ;
        printf("son: %d\n",j) ;
        sleep(1) ;
    }
    return NULL ;
}

int main()
{
    int res ;
    int i = 0 ;
    pthread_t my_thread ;
    res = pthread_create(&my_thread,NULL,son_thread_func,NULL);
    if (res){
        printf("thread create failed!\n") ;
        return 0 ;
    }
    while (1)
    {
        i++ ;
        printf("main: %d\n",i) ;
        sleep(1) ;
    }
    return 0 ;   
}
