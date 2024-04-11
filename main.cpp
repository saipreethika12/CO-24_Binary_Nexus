#include <iostream>
#include "Processor.h"
int main()
{
    Processor processor;
    int choice;
    std::cout << "Enter 1 for pipeline with forwarding and 2 for pipeline without forwarding" << std::endl;
    int pipeline;
    std::cin>>pipeline;
    std::cout<<"Policy 1:viaLRU"<<std::endl;
    std::cout<<"Policy 2:viaRandom"<<std::endl;

    int addi,mul,add,sub;
    std::cout << "Enter the latency for ADD:\n"
              << std::endl;
    std::cin >> add;
    std::cout << "Enter the latency for SUB:\n"
              << std::endl;
    std::cin >> sub;
    std::cout << "Enter the latency for ADDI:\n"
              << std::endl;
    std::cin >> addi;
    std::cout << "Enter the latency for MUL:\n"
              << std::endl;
    std::cin >> mul;

    processor.set_latencies(addi,add,mul,sub);
    processor.run(pipeline);

    return 0;
}