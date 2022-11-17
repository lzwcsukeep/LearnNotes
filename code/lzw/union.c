#include <stdio.h>
#include <string.h>
 
union Data {
   int i;
   float f;
   char str[20];
};
 
int main( ) {

   int a = 1 ;
   void *vpt = (void *)&a;
   printf("the value of vpt is : %p\n",vpt) ;
   printf("sizeof(int): %d\n",sizeof(int)) ;
   vpt ++ ;
   printf("the value of vpt is : %p\n",vpt) ;

   return 0;
}