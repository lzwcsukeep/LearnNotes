#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    void *shmaddr = NULL ;
    const char data[] = "Hello World!This is added by lzw\n" ;
    int shmid ;
    key_t key  = (key_t) 0x8989 ;
    long page_size = sysconf(_SC_PAGESIZE) ;
    int data_size = (strlen(data) + page_size - 1) & (~(page_size -1)) ;

    printf("data size: %d, page size : %ld\n",data_size, page_size);

    // 创建共享内存
    shmid = shmget(key,data_size,0644 | IPC_CREAT) ;

    if ( shmid == -1 ) { 
        perror( "shmget failed\n") ;
        exit(EXIT_FAILURE) ;
    }

    // 连接共享内存
    // 注意参数和返回值。
    shmaddr = shmat(shmid,NULL,0) ;
    if ( shmaddr == (void *) -1){
        perror("shmat failed\n") ;
        exit(EXIT_FAILURE) ;
    }

    //往共享内存中写数据
    memset(shmaddr,0,data_size) ;
    memcpy(shmaddr, &data,strlen(data)) ;

    if ( shmdt(shmaddr) == -1 ) {
        perror("shmdt failed\n") ;
        exit(EXIT_FAILURE) ;
    }
    printf("write done !\n") ;

    return 0 ;

}   
