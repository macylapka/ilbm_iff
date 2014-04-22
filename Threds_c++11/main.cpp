#include <iostream>
#include <thread>

void run1() {
  std::cout << "Hello from run1" << std::endl;
}

void run2(int x) {
  std::cout << "Hello from run2(" << x << ")" << std::endl; 
}

int main(int argc, char **argv) {

  std::thread t1(run1);
  std::thread t2(run2, 123);
  t2.detach();
  
  auto test = 123;  
  std::cout << "t1.joinable() " << t1.joinable() 
    << std::boolalpha << std::endl;
  std::cout << "t2.joinable() " << t2.joinable() 
    << std::boolalpha << std::endl;
  t1.join();
  std::cout << "t1.joinable() " << t1.joinable() 
    << std::boolalpha << std::endl;
  std::cout << "t2.joinable() " << t2.joinable() 
    << std::boolalpha << std::endl; 
  std::cout << "Hello from main" << std::endl;
  return 0;

}
