#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint> 
#include <ctime>
#include <bits/stdc++.h>
#ifndef PU_H
#define PU_H

class Core
{
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
  
   std::vector<std::pair<std::string, int>>getInstructions(){
    return instructions;
   }
   int getPC(){
    return PC;
   }
  int returnIndex(std::string r) {
    int i = -1;
    if (r == "x0" || r == "zero") {
        i = 0;
    } else if (r == "x1" || r == "ra") {
        i = 1;
    } else if (r == "x2" || r == "sp") {
        i = 2;
    } else if (r == "x3" || r == "gp") {
        i = 3;
    } else if (r == "x4" || r == "tp") {
        i = 4;
    } else if (r == "x5" || r == "t0") {
        i = 5;
    } else if (r == "x6" || r == "t1") {
        i = 6;
    } else if (r == "x7" || r == "t2") {
        i = 7;
    } else if (r == "x8" || r == "s0") {
        i = 8;
    } else if (r == "x9" || r == "s1") {
        i = 9;
    } else if (r == "x10" || r == "a0") {
        i = 10;
    } else if (r == "x11" || r == "a1") {
        i = 11;
    } else if (r == "x12" || r == "a2") {
        i = 12;
    } else if (r == "x13" || r == "a3") {
        i = 13;
    } else if (r == "x14" || r == "a4") {
        i = 14;
    } else if (r == "x15" || r == "a5") {
        i = 15;
    } else if (r == "x16" || r == "a6") {
        i = 16;
    } else if (r == "x17" || r == "a7") {
        i = 17;
    } else if (r == "x18" || r == "s2") {
        i = 18;
    } else if (r == "x19" || r == "s3") {
        i = 19;
    } else if (r == "x20" || r == "s4") {
        i = 20;
    } else if (r == "x21" || r == "s5") {
        i = 21;
    } else if (r == "x22" || r == "s6") {
        i = 22;
    } else if (r == "x23" || r == "s7") {
        i = 23;
    } else if (r == "x24" || r == "s8") {
        i = 24;
    } else if (r == "x25" || r == "s9") {
        i = 25;
    } else if (r == "x26" || r == "s10") {
        i = 26;
    } else if (r == "x27" || r == "s11") {
        i = 27;
    } else if (r == "x28" || r == "t3") {
        i = 28;
    } else if (r == "x29" || r == "t4") {
        i = 29;
    } else if (r == "x30" || r == "t5") {
        i = 30;
    } else if (r == "x31" || r == "t6") {
        i = 31;
    }

    return i; 
     }
    Core(std::ifstream &file) : file(file), PC(0)
    {
        for (int i = 0; i <= 31; ++i)
        {
            reg[i] = 0;
        }
        start_of_ins =3000;
    }
    // void Fetch();
    // void FetchWF();
    // void Decode();
    // void DecodeWF();
    // void Execute(std::vector<std::pair<std::string, std::string>> latch_IDRF);
    // void ExecuteWF(std::vector<std::pair<std::string, std::string>> latch_IDRF);
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
    int findLabelIndex(const std::string &label)
    {
        for (const auto &instruction : instructions)
        {
            std::istringstream iss(instruction.first);
            std::vector<std::string> tokens;
            std::string token;
            while (iss >> token)
            {
                tokens.push_back(token);
            }
            if (tokens.size() > 0 && tokens[0] == (label + ':'))
            {
                return instruction.second;
            }
        }
        std::cerr << "Error: Label not found - " << label << std::endl;
        return PC;
    }

    void handleSyscall(long long int syscallNumber)
    {
        switch (syscallNumber)
        {
        case 1:
            handlePrintSyscall();
            break;
        default:
            std::cerr << "Error: Unknown syscall number." << std::endl;
            break;
        }
        PC++;
    }
    
    void handlePrintSyscall()
    {
        long long int message = reg[10];
        std::cout << message << std::endl;
    }

    bool control_executions(std::string s,int op1,int op2)
    {
        
        if (s == "bne")
        {
            if (op1!=op2)
            {
                return true;
            }else{
                return false;
            }
        }
        else if (s == "blt")
        {
            if (op1<op2)
            {
                return true;
            }else{
                return false;
            }
        }
        else if (s == "bgt")
        {
            if (op1>op2)
            {
                return true;
            }else{
                return false;
            }
        }
        else if (s == "beq")
        {
            if (op1==op2)
            {  
                return true;
            }else{
                return false;
            }
        }
        else if (s == "bge")
        {
            if (op1>=op2)
            {
                return true;
            }else{
                return false;
            }
        }
   }


};


#endif
