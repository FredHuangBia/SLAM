#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <unistd.h>
void foo()
{
  int a;
  while(1){
  std::cin>>a;
  std::cout<<"I'am thread  "<<a<<std::endl;
  }
}

void bar(){
  while(1){
    std::cout<<"main"<<std::endl;

  }
}
int main()
{
  std::thread first (foo);     // spawn new thread that calls foo()
  // std::thread second (bar);
  // std::thread second (bar,0);  // spawn new thread that calls bar(0)

  std::cout << "main, foo  now execute concurrently...\n";
  // for(int i=0;i<10;i++){
  while(1){
    std::cout<<"main"<<std::endl;
    sleep(1);
  }
    first.join();
    // second.detach();
  // synchronize threads:
                // pauses until first finishes
  // second.join();               // pauses until second finishes

  std::cout << "foo and bar completed.\n";

  return 0;
}
