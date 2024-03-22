#include <iostream>
#include<fstream>
#include "PIPE_WOF.h"
#include "PIPE_WF.h"
class Processor
{
public:
    char RAM[4096];
    int clock = 0;
    bool visited[4096] = {0};
    void run(int x)
    {
        std::cout << "Sorted Array by bubble sort" << std::endl;
        std::ifstream instructionsFile("text.txt");
        if (!instructionsFile.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }

        Core core1(instructionsFile);
        core1.readInstructionsFromFile("bubble_sort.txt", RAM, visited);
        PIPE_WOF pwof;
        PIPE_WF pwf;
        
        if(x==1){
        pwf.Step_countWF(RAM);}
        else{
           pwof.Step_count(RAM);} 
        

        instructionsFile.close();
        std::ifstream instructionsFilee("text1.txt");
        if (!instructionsFilee.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }
        std::cout << "Sorted Array by selection sort" << std::endl;
        Core core2(instructionsFilee);
        core2.readInstructionsFromFile("selection.txt", RAM, visited);
       if(x==1){
        pwf.Step_countWF(RAM);}
        else{
           pwof .Step_count(RAM);} 

        instructionsFilee.close();
     }
};

