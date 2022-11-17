#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *ThreadFun(void* arg)
{
    if(arg ==NULL){
        printf("arg is NULL\n") ;
    }
    else
    {
        printf("%s\n",(char*)arg) ;
    }
    return NULL ;   
}

int main()
{
    int res ;
    const char * url = "http://c.biancheng.net" ;
    
    pthread_t my_thread1,my_thread2 ;
    // 第一个参数是线程类型变量，第三个参数是这个线程要执行的函数
    res = pthread_create(&my_thread1,NULL,ThreadFun,NULL);
    if (res != 0) {
        printf("thread create failed!\n") ;
        return 0 ;
    }
    //主线程休眠一会
    sleep(5) ;
    res = pthread_create(&my_thread2,NULL,ThreadFun,(void*)url) ;
    if (res != 0) {
        printf("thread2 create failed!\n") ;
        return 0 ;
    }
    sleep(5) ;
    printf("main thread finished!\n");
    return 0 ;
}
