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
public:
    char RAM[4096];
    int sets;
    int blocks;
    int offset;
    int clock = 0;
    bool visited[4096] = {0};
    Cache_simulator cacheSimulator;
    PIPE_WOF pwof;
    PIPE_WF pwf;


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
   std::map<std::string, std::string> parseInputFile(const std::string& filename) {
    std::map<std::string, std::string>config;
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return config;
    }

    std::string line;
    std::string section;
    while (std::getline(inputFile, line)) {
        if (!line.empty() && line.front() == '#') {
            section = line.substr(1);
            if (section == "Cache_Configuration") {
                config = getInputParameters(inputFile);
            }
        }
    }

    inputFile.close();
    return config;
}
    void set_cache(std::string filename){
         std::map<std::string, std::string> config = parseInputFile(filename);
         // Set Cache Configuration
         unsigned int cache_size = std::stoi(config["Cache_size"]);
         unsigned  block_size= std::stoi(config["Block_size"]);
         unsigned  associativity = std::stoi(config["Associativity"]);

         Cache_simulator cache(cache_size,block_size,associativity);
         this->cacheSimulator= cache;
       
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
        std::map<std::string, std::string> config = parseInputFile(inputFilename);
        //set_values(config["Cache_Configuration"]);
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
    }
};

#endif // PROCESSOR_H
