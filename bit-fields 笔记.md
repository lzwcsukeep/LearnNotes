### bit fields 使用

在结构体struct中可以指定成员的宽度，即成员变量所占的bit 数。
好处：在存储空间有限的时候可以节省资源
坏处：bit fields 的实现是与具体机器相关的因此可能影响程序的可移植性。

当存储空间紧张时使用。

**Need of bit fields in C programming language:**

- Reduces memory consumption.
- To make our program more efficient and flexible.
- Easy to Implement.

**Applications of Bit-fields:**

- If storage is limited, we can go for bit-field.
- When devices transmit status or information encoded into multiple bits for this type of situation bit-field is most efficient.
- Encryption routines need to access the bits within a byte in that situation bit-field is quite useful.

### 定义方式

和struct 的定义以及使用方式很像，不过在struct成员的后面用冒号指定成员的宽度。

```c
// C program to demonstrate use of Bit-fields 
#include <stdio.h>

// Space optimized representation of the date
struct date {
    // d has value between 0 and 31, so 5 bits
    // are sufficient
    int d : 5;

    // m has value between 0 and 15, so 4 bits
    // are sufficient
    int m : 4;

    int y;
};

int main()
{
    printf("Size of date is %lu bytes\n",
           sizeof(struct date));
    struct date dt = { 31, 12, 2014 };
    printf("before :Date is %d/%d/%d\n", dt.d, dt.m, dt.y);
    dt.d = 8;
    dt.m = 11 ;
    dt.y = 2022 ;
    printf("after :Date is %d/%d/%d\n", dt.d, dt.m, dt.y);
    return 0;

}
```

参考链接：

[Bit Fields in C - GeeksforGeeks](https://www.geeksforgeeks.org/bit-fields-c/)