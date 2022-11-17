信号量API

semget()

semop()

semctl()

#### int semget(key_t key, int num_sems,int sem_flags)

- key   -  整数值key,系统用来唯一标识信号量，不相关进程可以通过它访问一个信号量

- num_sems   - 信号量数目，几乎总是1

- sem_flags   -  一组标志

- 返回值    -  返回信号量唯一标识符sem_id。其他函数用sem_id 确定需要操作的信号量。

#### int semop(int sem_id, struct sembuf* sem_opa, size_t        num_sem_ops)

- sem_id 是由semget() 返回的信号量标识符

- sembuf 结构的定义：
  
  ```c
  struct sembuf{
      short sem_num ;  // 除非使用一组信号量，否则它为0
      short sem_op ;   // 信号量在一次操作中需要改变的数据，通常是两个数，
                       // 一个-1,即P操作。一个是+1,即V操作。
  
      short sem_flag   // 通常为SEM_UNDO，是操作系统跟踪信号，
                       //并在进程没有释放该信号量时，操作系统释放该信号量
  }
  ```

#### int semctl(int sem_id,int sem_num,int command,...)

如果有第四个参数，它通常是一个union semun的结构，定义如下：

```c
union semun{
    int val ;
    struct semid_ds *buf ;
    unsigned short* array ;
};
```

- sem_id

- sem_num

- command 

command 通常是下面两个值中的一个：

SETVAL : 用来把信号量初始化为一个已知的值

IPC_RMID:  用于删除一个已经无需继续i使用的信号量标识符。
