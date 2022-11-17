#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
void * threadFunc(void * arg)
{
    sleep(5) ;
    std::cout << "new:thread function :: start " << std::endl;
    sleep(5) ;
    std::cout << "new:thread function :: end " << std::endl ;
    return NULL ;
}
int main()
{
    printf("threadFunc address:%p\n,%p\n,%p\n",threadFunc,&threadFunc,&&threadFunc);
    printf("----------------------------------------\n") ;
    pthread_t threadID;
    int err = pthread_create(&threadID,NULL,&threadFunc,NULL) ;
    if (err)
    {
        std::cout << "main:Thread create failed:" << strerror(err) ;
        return err ;
    }
    else {
        std::cout << "main:Thread create with ID :" << threadID << std::endl ;
    }

    std::cout << "main:Waiting for thread to exit" << std::endl ;
    //暂停主调线程执行，等待参数指定的线程执行结束。
    err = pthread_join(threadID,NULL) ;
    if(err){
        printf("main:Failed to join Thread: %s\n",strerror(err)) ;
        return err ;
    }
    printf("main:Exiting Main\n") ;
    return 0 ;
}
