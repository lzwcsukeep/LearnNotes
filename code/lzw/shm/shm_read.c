#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    void *shmaddr = NULL;
    const char data[] = "Hello World\n" ;
    int shmid ;
    key_t key = (key_t) 0x8989 ;
    long page_size = sysconf( _SC_PAGESIZE) ;
   
    int data_size = (strlen(data) + page_size -1 ) & (~(page_size -1 )) ;
    printf("data size :%d, page size: %ld\n",data_size,page_size) ;

    // 1.get the shared memory 
    shmid = shmget(key ,data_size,0644 | IPC_CREAT) ;
    if (shmid == -1){
        perror("shmget failed\n");
        exit(EXIT_FAILURE) ;
    }

    // 2.attach to the shared memory.
    shmaddr = shmat(shmid,NULL,0); 

    if (shmaddr == (void *)-1) {
        perror("shmat failed\n");
        exit(EXIT_FAILURE) ;
    }
    // 3. read data from shared memory.
    
    printf("read from shared memory: %s\n",(char *) shmaddr) ;
    // 4. detach shared memory
    if (shmdt(shmaddr) == -1) {
        perror("shmdt failed\n");
        exit(EXIT_FAILURE) ;
        
    }
  // 5. delete shared memory .
  
   if ( shmctl(shmid ,IPC_RMID,0) == -1){
       perror("shmctl delete shared memory failed\n");
       exit(EXIT_FAILURE) ;
       
   }
    return 0;
}
