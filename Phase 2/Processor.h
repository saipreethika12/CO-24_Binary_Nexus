#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <fstream>
#include "PIPE_WOF.h"
#include "PIPE_WF.h"

class Processor
{
public:
    char RAM[4096];
    int clock = 0;
    bool visited[4096] = {0};
    PIPE_WOF pwof;
    PIPE_WF pwf;
    void set_latencies(int addi_lat, int add_lat, int mul_lat, int sub_lat)
    {
        pwf.latency_map["ADDI"] = addi_lat;
        pwf.latency_map["ADD"] = add_lat;
        pwf.latency_map["MUL"] = mul_lat;
        pwf.latency_map["SUB"] = sub_lat;
    }
    void run(int x)
    {
        std::ifstream instructionsFile("text.txt");
        if (!instructionsFile.is_open())
        {
            std::cerr << "Error opening file 'text.txt'." << std::endl;
            return;
        }

        // Assuming PIPE_WOF extends Core
        pwof.readInstructionsFromFile("text.txt", RAM, visited);

        if (x == 1)
        {
            pwof.Step_count(RAM);
        }
        else
        {

            pwf.readInstructionsFromFile("text.txt", RAM, visited);
            pwf.Step_countWF(RAM);
        }

        instructionsFile.close();

        // std::ifstream instructionsFilee("text1.txt");
        // if (!instructionsFilee.is_open()) {
        //     std::cerr << "Error opening file 'text1.txt'." << std::endl;
        //     return;
        // }

        // PIPE_WOF pwof2; // Assuming PIPE_WOF extends Core
        // pwof2.readInstructionsFromFile("selection.txt", RAM, visited);

        // if (x == 1) {
        //     pwof2.Step_count(RAM);
        // } else {
        //     PIPE_WF pwf;
        //     pwf.Step_countWF(RAM);
        // }

        // instructionsFilee.close();
    }
};

#endif // PROCESSOR_H
