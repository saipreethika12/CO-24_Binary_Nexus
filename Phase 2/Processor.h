#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <fstream>
#include <cmath>
#include "PIPE_WOF.h"
#include "PIPE_WF.h"

class Processor
{
public:
    char RAM[4096];
    int sets;
    int blocks;
    int offset;
    char ***cache; // cache is a 3D array of pointers to lines in the RAM
    int clock = 0;
    bool visited[4096] = {0};
    PIPE_WOF pwof;
    PIPE_WF pwf;
    void init_cache(int s, int b, int o)
    {
        sets = s;
        blocks = b;
        offset = o;
        cache = new char **[sets];
        for (int i = 0; i < sets; i++)
        {
            cache[i] = new char *[blocks];
            for (int j = 0; j < blocks; j++)
            {
                cache[i][j] = new char[offset];

                for (int k = 0; k < offset; k++)
                {
                    cache[i][j][k] = '\0';
                }
            }
        }
    }
    void set_latencies(int addi_lat, int add_lat, int mul_lat, int sub_lat)
    {
        pwf.latency_map["ADDI"] = addi_lat;
        pwf.latency_map["ADD"] = add_lat;
        pwf.latency_map["MUL"] = mul_lat;
        pwf.latency_map["SUB"] = sub_lat;
        pwof.latency_map["ADDI"] = addi_lat;
        pwof.latency_map["ADD"] = add_lat;
        pwof.latency_map["MUL"] = mul_lat;
        pwof.latency_map["SUB"] = sub_lat;
    }
    std::map<std::string, std::string> getInputParameters(std::ifstream &inputFile)
    {
        std::map<std::string, std::string> parameters;
        std::string line;
        std::string key, value;

        while (std::getline(inputFile, line) && !line.empty() && line.front() != '#')
        {
            if (line.find(':') != std::string::npos)
            {
                std::istringstream iss(line);
                std::getline(iss, key, ':');
                std::cout << "Enter value for " << key << ": ";
                std::getline(std::cin, value);
                parameters[key] = value;
            }
        }

        return parameters;
    }
    std::map<std::string, std::map<std::string, std::string>> parseInputFile(const std::string &filename)
    {
        std::map<std::string, std::map<std::string, std::string>> config;

        std::ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            std::cerr << "Error opening input file." << std::endl;
            return config;
        }

        std::string line;
        std::string section;
        while (std::getline(inputFile, line))
        {
            if (!line.empty() && line.front() == '#')
            {
                section = line.substr(1);
                if (section == "Cache_Configuration" || section == "Memory_access")
                {
                    config[section] = getInputParameters(inputFile);
                }
            }
        }

        inputFile.close();
        return config;
    }
    void set_values(std::map<std::string, std::string> cache_section){
        int block_size=stoi(cache_section["Block_size"]);
        int cache_size=stoi(cache_section["Cache_size"]);
        int associativity=stoi(cache_section["Associativity"]);
        int n= log10(block_size) / log10(2);
        this->offset= n;
        int N= (cache_size*(int)pow(2,10))/block_size;
        this->blocks=N;
        this->sets=N/associativity;


    }
    void run(int x)
    {
        std::ifstream instructionsFile("input.txt");
        if (!instructionsFile.is_open())
        {
            std::cerr << "Error opening file 'text.txt'." << std::endl;
            return;
        }
        std::string inputFilename = "input.txt";
        std::map<std::string, std::map<std::string, std::string>> config = parseInputFile(inputFilename);
        set_values(config["Cache_Configuration"]);
        pwof.readInstructionsFromFile("input.txt", RAM, visited);

        if (x == 1)
        {
            pwof.Step_count(RAM);
        }
        else
        {

            pwf.readInstructionsFromFile("input.txt", RAM, visited);
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
