## Shared Memory API

- shmget()    分配一个共享内存块

- shmat()       进程连接到共享内存块

- shmdt()      进程与共享内存块分离

- shmctl()     控制共享内存块

一个共享内存块由一个有着唯一id 的控制结构描述，这个结构指向物理内存的一个区域。这个共享内存块的标识符叫做`shmid`。

> A shared memory segment is described by a control structure with a unique ID that points to an area of physical memory. The identifier of the segment is called the shmid.

---



#### shmget()原型：

```cpp
int shmget(key_t key, size_t size, int shmflg);
```

返回：

成功返回int shmid ,获取的共享内存块的唯一标识。失败返回-1。

- key -

- size - 共享内存块的大小（bytes）

- shmflg - 共享内存块的访问权限

---



### shmat()原型：

```cpp
void *shmat(int shmid, const void *shmaddr, int shmflg);
```

#### shmat() 使进程连接到`shmid` 指定的共享块，返回共享块的首地址。

### 参数

- shmid 
  
  - 有效共享块的唯一标识

- shmaddr
  
  - shmaddr 指定共享块映射到该进程地址空间的地址，如果为NULL，则系统选择一个合适的地址连接。 

### 返回值

   如果成功`shmat()` 返回共享内存的地址，出错返回（void \*）-1

---



### shmdt()原型：将共享内存分离当前进程

```cpp
int shmdt(const void *shmaddr);
```

### 参数

shmat()成功的返回值

### 返回值

成功返回0，出错返回-1.

---



### shmctl 原型

> shmctl() is used to alter the permissions and other characteristics of a shared memory segment.

```cpp
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

- shmid  : 共享块的标识符

- cmd :  下述之一
  
  - **SHM_LOCK**
    
    -- Lock the specified shared memory segment in memory. The process must have the effective ID of superuser to perform this command.
    
    **SHM_UNLOCK**
    
    -- Unlock the shared memory segment. The process must have the effective ID of superuser to perform this command.
    
    **IPC_STAT**
    
    -- Return the status information contained in the control structure and place it in the buffer pointed to by buf. The process must have read permission on the segment to perform this command.
    
    **IPC_SET**
    
    -- Set the effective user and group identification and access permissions. The process must have an effective ID of owner, creator or superuser to perform this command.
    
    **IPC_RMID**
    
    -- Remove the shared memory segment.

- buf 

        -- The buf is a sructure of type struct shmid_ds which is defined in <sys/shm.h>






































