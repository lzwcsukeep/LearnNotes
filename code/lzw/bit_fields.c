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
    unsigned int b : 1 ;
    int y;
    
};
  
int main()
{
    printf("Size of date is %lu bytes\n",
           sizeof(struct date));
    struct date dt = { 31, 12, 1,2014 };
    printf("before :Date is %d/%d/%d/%d\n", dt.d, dt.m, dt.y,dt.b);
    dt.d = 8;
    dt.m = 11 ;
    dt.y = 2022 ;
    dt.b = 0 ;
    printf("after :Date is %d/%d/%d/%d\n", dt.d, dt.m, dt.y,dt.b);
    return 0;

}
