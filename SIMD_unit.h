#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint> 
#include <ctime>
#include <bits/stdc++.h>
class SIMD_unit{
    public:
     std::unordered_map<std::string, long long int> labelToAddress;
   // std::unordered_map<std::string, long long int> reg;
   long long int reg[32];
   int  reg_state[32]={0};
    int PC;
    std::vector<std::pair<std::string, int>> instructions;
    std::ifstream &file;
   int start_of_ins;
   std::map<int,int> ins_map;
    public:
      void readInstructionsFromFile(const std::string &filename, char *RAM, bool *vis)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }

        int index = 0;
        std::string line;
        bool isDataSection = false;
        bool istextsec = false;
        int pointer_of_ins = start_of_ins;

        while (getline(file, line))
        {
            if (line == ".data")
            {
                isDataSection = true;
                continue;
            }
            if (line == ".text")
            { 
                istextsec = true;
                continue;
            }
            if (istextsec)
            {
                instructions.push_back(make_pair(line, index));
                            ins_map[index]=pointer_of_ins;
                           int val =index ;
                            for (int i = 0; i < 4; i++)
                            {
                                int t = 0;
                                for (int j = 0; j < 8; j++)
                                {
                                    if ((val >> (8 * i + j) & 1 == 1))
                                        t += pow(2, j);
                                }
                                RAM[pointer_of_ins] = t;
                                vis[pointer_of_ins] = 1;
                                pointer_of_ins++;
                            }
                              index++;
                continue;
            }
            if (isDataSection && !istextsec)
            {
                std::istringstream iss(line);
                std::string label;
                iss >> label;
                if (label.back() == ':')
                {
                    label.pop_back();

                    std::string value;
                    int x;
                    labelToAddress[label] = get_index(vis);
                    // // if(label=="arr"){
                    //         labelToAddress[label] = 2000;}
                    //         if(label=="arr_len"){
                    //             labelToAddress[label]=400;
                    //         }
                    x = labelToAddress[label];
                    std::cout<<"The index i chose"<<x<<std::endl;
                    while (iss >> value)
                    {
                        if (value == ".word")
                            continue;
                        if (isInteger(value))
                        {
                            int val = std::stoi(value);
                            for (int i = 0; i < 4; i++)
                            {
                                int t = 0;
                                for (int j = 0; j < 8; j++)
                                {
                                    if ((val >> (8 * i + j) & 1 == 1))
                                        t += pow(2, j);
                                }
                                RAM[x] = t;
                                vis[x] = 1;
                                x++;
                            }
                        }
                    }
                }
            }

            else
            {
                std::cerr << line << std::endl;
                std::cerr << "Error: Invalid .word instruction format." << std::endl;
            }
        }

        file.close();
    }
    int get_index(bool *vis)
    {
        int k = rand() % 4096;
        while (vis[k] != 0)
        {
            k = (k + 1000) % 4096;
        }
        return k;
    }
       bool isInteger(const std::string &value)
    {
        // Check if the string is empty or contains only whitespace
        if (value.empty() || value.find_first_not_of(" \t\v\n") == std::string::npos)
            return false;

        // Check for a negative sign at the beginning
        size_t start = 0;
        if (value[0] == '-')
        {
            if (value.length() == 1) // Just a negative sign without any digits
                return false;
            start = 1;
        }

        // Check if the remaining characters are digits
        return value.find_first_not_of("0123456789", start) == std::string::npos;
    }
    void execute(){
        while(PC<instructions.size()){
            std::string ins = instructions[PC].first;
            
        }
    }
};