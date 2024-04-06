#include <iostream>
#include "Processor.h"
int main()
{
      std::cout<<"ls"<<std::endl;
    Processor processor;
  
    int choice;
    // std::cout << "Enter 1 for pipeline with forwarding and 2 for pipeline without forwarding" << std::endl;
    // int addi,mul,add,sub;
    // std::cout << "Enter the latency for ADD:\n"
    //           << std::endl;
    // std::cin >> add;
    // std::cout << "Enter the latency for SUB:\n"
    //           << std::endl;
    // std::cin >> sub;
    // std::cout << "Enter the latency for ADDI:\n"
    //           << std::endl;
    // std::cin >> addi;
    // std::cout << "Enter the latency for MUL:\n"
    //           << std::endl;
    // std::cin >> mul;
    processor.set_latencies(1,1,1,1);
    
    processor.run(1);

    return 0;
}
