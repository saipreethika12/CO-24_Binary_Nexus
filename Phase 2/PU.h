#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint> 
#include <ctime>
#include <bits/stdc++.h>

#define CORE_H
std::vector<std::pair<std::string, std::string>> latch_IF;
std::vector<std::pair<std::string, std::string>> latch_IDRF;
std::vector<std::pair<std::string, std::string>> latch_EXE;
std::vector<std::pair<std::string, std::string>> latch_MEM;
std::map<std::string, int> latency_map;

// std::vector<std::pair<std::string, std::string>> latch_WB;
bool ishazard_notified = false;
int stalls = 0;
bool eof = false;
int ins = 0;
int prevPC = 0;
int prevprevPC = 0;
bool mis_predict = false;
bool stall_flag = false;
bool stall_flag2 = false;
bool branch_flag = false;
bool stall_mul = false;
bool executed_branch = false;
int count = 1;
int deof = false;
int loop = 0;
bool keep_going = true;
int v = 50;

std::vector<std::vector<std::string>> pip(100, std::vector<std::string>(1000, " "));
std::vector<std::string> ins_type1 = {"add", "sub", "and", "or", "slt","mul"};
std::vector<std::string> ins_type2 = {"addi", "andi", "ori", "sll", "srl", "slli"};
std::vector<std::string> ins_type3 = {"bne", "beq", "bge", "bgt", "blt"};
std::vector<std::string> ins_type4 = {"lw", "sw", "la"};
std::vector<std::string> ins_type5 = {"j", "jal", "jalr"};
std::vector<std::string> ins_type6 = {"lui"};
void imbibe_stall()
{
    std::cout << "STAll"
              << " ";
}
std::string search_latch(const std::string &opcode, const std::vector<std::pair<std::string, std::string>> &latch_IDRF)
{
    for (const auto &pair : latch_IDRF)
    {
        if (pair.first == opcode)
        {
            return pair.second; // Return the value if opcode matches
        }
    }
    return ""; // Return empty string if opcode is not found
}
bool isHazard(std::string rs, std::vector<std::pair<std::string, std::string>> v)
{
    if (latch_MEM.size() > 0)
    {
        std::cout<<"CAME HERE"<<" "<<(rs == search_latch("rd", v))<<std::endl;
        return rs == search_latch("rd", v);
    }
    else
        return false;
}
bool predict_branch(){
    return false;
}
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

//     void executeInstructions(char *RAM)
//     {
//         while (PC < instructions.size())
//         {
//           //  std::cout << PC << std::endl;
//             std::string instruction = instructions[PC].first;
//             if (instruction.substr(0, 5) == "ecall")
//             {
//                 int syscallNumber = reg[17];
//                 handleSyscall(syscallNumber);
//             }
//             else
//             {
//                 execute(instruction, RAM);
//             }
//         }

//         int s = 0;
//         int t = 5;
//         int index = 2000;
//         while (t--)
//         {
//             s = 0;
//             for (int i = 0; i < 4; i++)
//             {

//                 int c = RAM[index];
//                 for (int j = 0; j < 8; j++)
//                 {
//                     if (i == 3 && j == 7)
//                     {
//                         if (c >> 7 & 1 == 1)
//                             s -= pow(2, 31);
//                         continue;
//                     }
//                     if (c >> j & 1 == 1)
//                         s += pow(2, 8 * i + j);
//                 }
//                 index += 1;
//             }
//             // cout<<s<<" "<<endl;
//         }
//     }

// public:
//    void Memory(std::vector<std::pair<std::string, std::string>> latch_EXE, char* RAM);
//    void MemoryWF(std::vector<std::pair<std::string, std::string>> latch_EXE, char* RAM);
//    void WriteBack(std::vector<std::pair<std::string, std::string>> latch_MEM);
//    void WriteBackWF(std::vector<std::pair<std::string, std::string>> latch_MEM);
//    void Step_countWF(char *RAM);
//    void Step_count(char *RAM);
//    void execute(const std::string &instruction, char *RAM)
//    {
//        std::istringstream iss(instruction);
//        // std:: cout<<instruction<<std::endl;
//        std::vector<std::string> tokens;
//        std::string token;
//        while (iss >> token)
//        {

//            tokens.push_back(token);
//            // std:: cout<<token<<" ";
//        }

//        if (tokens.empty())
//        {
//            std::cerr << "Error: Empty instruction." << std::endl;
//            return;
//        }

//        std::string opcode = tokens[0];
//        //  std::cout << opcode.back() << std::endl;
//        std::string rd;
//        if (tokens.size() > 1)
//        {
//            rd = tokens[1];
//        }

//        if (opcode.back() == ':')
//        {
//        }
//        else if (opcode == "lw")
//        {
//            // std::  cout<<"e"<<endl;
//            std::string baseRegister;
//            std::string offset;
//            int x;
//            if (labelToAddress.find(tokens[2]) != labelToAddress.end())
//            {
//                // reg[rd]=labelToAddress[tokens[2]];
//                offset = "0";
//                x = labelToAddress[tokens[2]];
//            }
//            else
//            {
//                std::string offsetBase = tokens[2];
//                size_t openParen = offsetBase.find_first_of('(');
//                if (openParen == std::string::npos)
//                {
//                    std::cerr << "Error: Invalid offset(base) format." << std::endl;
//                    return;
//                }
//                std::string r = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);
//                x = reg[returnIndex(r)];
//                offset = offsetBase.substr(0, openParen);
//            }
//            int index = 0;

//            int s = 0;
//            for (int i = 0; i < 4; i++)
//            {

//                int c = RAM[stoi(offset) + x + i];
//                for (int j = 0; j < 8; j++)
//                {
//                    if (i == 3 && j == 7)
//                    {
//                        if (c >> 7 & 1 == 1)
//                            s -= pow(2, 31);
//                        continue;
//                    }
//                    if (c >> j & 1 == 1)
//                        s += pow(2, 8 * i + j);
//                }
//            }
//            reg[returnIndex(rd)] = s;
//        }

//        else if (opcode == "sw")
//        {
//            std::string offsetBase = tokens[2];
//            std::size_t openParen = offsetBase.find_first_of('(');
//            if (openParen == std::string::npos)
//            {
//                std::cerr << "Error: Invalid offset(base) format." << std::endl;
//                return;
//            }
//            std::string baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);
//            std::string offset = offsetBase.substr(0, openParen);
//            // int index = reg[baseRegister] + stoi(offset)/4; RAM[index] = reg[rd];
//            int s = reg[returnIndex(rd)];
//            for (int i = 0; i < 4; i++)
//            {
//                int t = 0;
//                for (int j = 0; j < 8; j++)
//                {
//                    if ((s >> (8 * i + j) & 1 == 1))
//                        t += pow(2, j);
//                }
//                RAM[std::stoi(offset) + reg[returnIndex(baseRegister)] + i] = t;
//            }
//        }
//        else if (opcode == "la")
//        {
//            if (labelToAddress.find(tokens[2]) != labelToAddress.end())
//            {
//                reg[returnIndex(rd)] = labelToAddress[tokens[2]];
//            }
//        }

//        else if (opcode == "li")
//        {
//            int iv = stoi(tokens[2]);
//            reg[returnIndex(rd)] = iv;
//        }
//        else if (opcode == "slli")
//        {
//            std::string rs1 = tokens[2];
//            int b = stoi(tokens[3]);
//            reg[returnIndex(rd)] = reg[returnIndex(rs1)] << b;
//        }
//        else if (opcode == "bne" || opcode == "blt" || opcode == "bgt" || opcode == "beq" || opcode == "bge")
//        {
//            // control_executions(opcode, tokens);
//         }
//         else if (opcode == "jalr")
//         {
//             reg[returnIndex(rd)] = PC + 1;
//             PC = reg[returnIndex(tokens[2])];
//         }
//         else if (opcode == "j")
//         {
//             // Jump to the label
//             PC = findLabelIndex(tokens[1]);
//         }
//         else if (opcode == "add" || opcode == "sub" || opcode == "addi")
//         {
//             // Arithmetic instructions
//           //  executeArithmeticInstruction(tokens);
//         }
//         else
//         {
//             std::cerr << "Error: Unknown opcode." << std::endl;
//           //  std::cout << opcode << std::endl;
//         }

//         PC += 1;
//     }

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

    // void executeArithmeticInstruction(const std::vector<std::string> &tokens)
    // {
    //     std::string opcode = tokens[0];
    //     std::string rd = tokens[1];

    //     if (opcode == "add")
    //     {
    //         int op1 = reg[returnIndex(tokens[2])];
    //         int op2 = reg[returnIndex(tokens[3])];
    //         reg[returnIndex(rd)] = op1 + op2;
    //     }
    //     else if (opcode == "sub")
    //     {
    //         int op1 = reg[returnIndex(tokens[2])];
    //         int op2 = reg[returnIndex(tokens[3])];
    //         reg[returnIndex(rd)] = op1 - op2;
    //     }
    //     else if (opcode == "addi")
    //     {
    //         int op1 = reg[returnIndex(tokens[2])];
    //         int op2 = stoi(tokens[3]);
    //         reg[returnIndex(rd)]= op1 + op2;
    //     }
    // }
};

// class Processor
// {
// public:
//     char RAM[4096];
//     int clock = 0;
//     bool visited[4096] = {0};
//     void run(int x)
//     {
//         std::cout << "Sorted Array by bubble sort" << std::endl;
//         std::ifstream instructionsFile("text.txt");
//         if (!instructionsFile.is_open())
//         {
//             std::cerr << "Error opening file." << std::endl;
//             return;
//         }

//         Core core1(instructionsFile);
//         core1.readInstructionsFromFile("bubble_sort.txt", RAM, visited);
//         if(x==1){
//         core1.Step_countWF(RAM);}
//         else{
//             core1.Step_count(RAM);} 
        

//         instructionsFile.close();
//         std::ifstream instructionsFilee("text1.txt");
//         if (!instructionsFilee.is_open())
//         {
//             std::cerr << "Error opening file." << std::endl;
//             return;
//         }
//         std::cout << "Sorted Array by selection sort" << std::endl;
//         Core core2(instructionsFilee);
//         core2.readInstructionsFromFile("selection.txt", RAM, visited);
//        if(x==1){
//         core2.Step_countWF(RAM);}
//         else{
//             core2.Step_count(RAM);} 

//         instructionsFilee.close();
//     }
// };

// // int main()
// // {
// //     Processor processor;
// //     // cout << "Register values:" << endl;
// //     processor.run();

// //     return 0;
// // }
// #endif