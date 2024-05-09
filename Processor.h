#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <iostream>
#include <fstream>
#include <cmath>
#include "PIPE_WOF.h"
#include "PIPE_WF.h"
#include "cache_simulator.h"

class Processor
{
private:
    char RAM[4096];
    int sets;
    int blocks;
    int offset;
    int clock = 0;
    bool visited[4096] = {0};
    Cache_simulator cacheSimulator;
    PIPE_WOF pwof;
    PIPE_WF pwf;

public:
    Processor() : cacheSimulator(1, 1, 1, 1, 1, 1,1,1,1,1)
    {
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
    std::map<std::string, std::string> parseInputFile(const std::string &filename, const std::string &sectionName)
    {
        std::map<std::string, std::string> config;
        std::ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            std::cerr << "Error opening input file." << std::endl;
            return config;
        }

        std::string line;
        std::string section;
        bool sectionFound = false;
        while (std::getline(inputFile, line))
        {
            if (!line.empty() && line.front() == '#')
            {
                section = line.substr(1);
                if (section == sectionName)
                {
                    sectionFound = true;
                    continue;
                }
            }
            if (sectionFound)
            {
                if (line.find(':') != std::string::npos)
                {
                    std::istringstream iss(line);
                    std::string key, value;
                    std::getline(iss, key, ':');
                    std::getline(iss, value);
                    // Trim leading and trailing whitespace from the value
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
                    config[key] = value;
                }
            }
        }
        std::cout << "conf" << std::endl;
        inputFile.close();
        return config;
    }

   void set_cache(std::string filename)
    {
        std::map<std::string, std::string> config = parseInputFile(filename, "Cache_Configuration");
        // Set Cache Configuration
        unsigned int cache_size_l1 = std::stoi(config["Cache_size_l1"]);
        unsigned int block_size_l1 = std::stoi(config["Block_size_l1"]);
        unsigned int associativity_l1 = std::stoi(config["Associativity_l1"]);
        unsigned int cache_latency_l1 = std::stoi(config["Cache_latency_l1"]);
        unsigned int cache_size_l2 = std::stoi(config["Cache_size_l2"]);
        unsigned int block_size_l2 = std::stoi(config["Block_size_l2"]);
        unsigned int associativity_l2 = std::stoi(config["Associativity_l2"]);
        unsigned int cache_latency_l2 = std::stoi(config["Cache_latency_l2"]);
        unsigned int memory_latency = std::stoi(config["Memory_latency"]);
        int policy_num=std::stoi(config["Policy"]);
        Cache_simulator cache(cache_size_l1, block_size_l1, associativity_l1, cache_latency_l1,cache_size_l2, block_size_l2, associativity_l2, cache_latency_l2,memory_latency,policy_num);
        // Cache_simulator cache(, 4, 16, 2, 100);
        this->cacheSimulator = cache;
    }
    void run(int x)
    {
        std::string inputFilename = "cache_config.txt";
   
        set_cache(inputFilename);

        std::ifstream instructionsFile1("bubble_sort.txt");
        if (!instructionsFile1.is_open())
        {
            std::cerr << "Error opening file " << std::endl;
            return;
        }
        pwof.readInstructionsFromFile("bubble_sort.txt", RAM, visited);
        if (x == 2)
        {
            pwof.Step_count(RAM, &cacheSimulator);
        }
        else if (x == 1)
        {

            pwf.readInstructionsFromFile("bubble_sort.txt", RAM, visited);
            std::cout<<"mm"<<std::endl;
            pwf.Step_countWF(RAM, &cacheSimulator);
            std::cout<<"mmss"<<std::endl;
        }
        else
        {
            std::cout << "Invalid ip" << std::endl;
        }

        instructionsFile1.close();
        }

    //     std::ifstream instructionsFile2("selection.txt");
    //     if (!instructionsFile2.is_open())
    //     {
    //         std::cerr << "Error opening file " << std::endl;
    //         return;
    //     }
    //     pwof.readInstructionsFromFile("selection.txt", RAM, visited);
    //     if (x == 2)
    //     {
    //         pwof.Step_count(RAM, &cacheSimulator);
    //     }
    //     else if (x == 1)

    //     {
    //         pwf.readInstructionsFromFile("selection.txt", RAM, visited);
    //         pwf.Step_countWF(RAM, &cacheSimulator);
    //     }
    //     else
    //     {
    //         std::cout << "Invalid ip" << std::endl;
    //     }

    //     instructionsFile2.close();
    // }
};

#endif // PROCESSOR_H
