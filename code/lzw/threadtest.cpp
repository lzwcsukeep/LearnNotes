#include <iostream>
#include <thread>

void foo(){
    //do something here in foo
    std::cout << "thread in foo" <<std::endl;
}

void bar(int x){
    //do something here in bar
    std::cout << "thread in bar(int x), param x = "<< x << std::endl ;
}
int main()
{
    std::thread first(foo) ;
    std::thread second(bar,0) ;

    std::cout << "main foo bar execute concurrently... \n" ;

    first.join() ;
    second.join();

    std::cout << "foo and bar completed.\n" ;
    return 0 ;
}
