#include <iostream>
#include "Processor.h"
int main()
{
    Processor processor;
    int choice;
    std::map <std::string,int> latency_map;
    std::cout << "Enter 1 for pipeline with forwarding and 2 for pipeline without forwarding" << std::endl;
    int x;
    std::cout << "Enter the latency for ADD:\n"
              << std::endl;
    std::cin >> x;
    latency_map["ADD"] = x;
    std::cout << "Enter the latency for SUB:\n"
              << std::endl;
    std::cin >> x;
    latency_map["SUB"] = x;
    std::cout << "Enter the latency for ADDI:\n"
              << std::endl;
    std::cin >> x;
    latency_map["ADDI"] = x;
    processor.run(1);


    return 0;
}