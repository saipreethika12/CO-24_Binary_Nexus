#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <ctime>
#include <bits/stdc++.h>

class Core
{
private:
    std::unordered_map<std::string, long long int> labelToAddress;
    std::unordered_map<std::string, long long int> reg;
    int PC;
    std::vector<std::pair<std::string, int>> instructions;
    std::ifstream &file;

public:
    Core(std::ifstream &file) : file(file), PC(0)
    {
        for (int i = 0; i <= 31; ++i)
        {
            reg["x" + std::to_string(i)] = 0;
        }
    }

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
            size_t commentPos = line.find('#');
            if (commentPos != std::string::npos)
            {
                line = line.substr(0, commentPos); // Remove everything after #
            }

            // Trim leading and trailing whitespaces
            line = trim(line);

            // Skip empty lines
            if (line.empty())
            {
                continue;
            }
            if (line == ".data")
            {
                // std:: cout<<"d";
                isDataSection = true;

                continue;
            }
            if (line == ".text")
            {
                // std:: cout<<"t";
                istextsec = true;
                continue;
            }
            if (istextsec)
            {
                // std:: cout<<line<<index<<std::endl;
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
                    while (iss >> value)
                    {
                        if (value == ".word")
                            continue;
                        if (isInteger(value))
                        {
                            int val = std::stoi(value);
                            // val 8 8  8 8
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
                // std::cout << line[0] << std::endl;
                std::cerr << "Error: Invalid .word instruction format." << std::endl;
            }
        }

        file.close();
    }
    std::string trim(const std::string &str)
    {
        size_t first = 0;
        size_t last = str.length() - 1;

        // Find the index of the first non-whitespace character
        while (first < str.length() && (str[first] == ' ' || str[first] == '\t' || str[first] == '\r' || str[first] == '\n'))
        {
            ++first;
        }

        // If the string contains only whitespace characters, return an empty string
        if (first == str.length())
        {
            return "";
        }

        // Find the index of the last non-whitespace character
        while (last > first && (str[last] == ' ' || str[last] == '\t' || str[last] == '\r' || str[last] == '\n'))
        {
            --last;
        }

        // Extract the trimmed substring from the original string
        return str.substr(first, last - first + 1);
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
        long long int message = reg["x10"];
        std::cout << message << std::endl;
    }

    void executeInstructions(char *RAM)
    {
        while (PC < instructions.size())
        {
            //  std::cout << PC << std::endl;
            std::string instruction = instructions[PC].first;
            if (instruction.substr(0, 5) == "ecall")
            {
                int syscallNumber = reg["x17"];
                handleSyscall(syscallNumber);
            }
            else
            {
                execute(instruction, RAM);
            }
        }

        int s = 0;
        int t = 5;
        int index = 2000;
        while (t--)
        {
            s = 0;
            for (int i = 0; i < 4; i++)
            {

                int c = RAM[index];
                for (int j = 0; j < 8; j++)
                {
                    if (i == 3 && j == 7)
                    {
                        if (c >> 7 & 1 == 1)
                            s -= pow(2, 31);
                        continue;
                    }
                    if (c >> j & 1 == 1)
                        s += pow(2, 8 * i + j);
                }
                index += 1;
            }
            // cout<<s<<" "<<endl;
        }
    }

private:
    void execute(const std::string &instruction, char *RAM)
    {
        std::istringstream iss(instruction);
        // std:: cout<<instruction<<std::endl;
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token)
        {

            tokens.push_back(token);
            // std:: cout<<token<<" ";
        }

        if (tokens.empty())
        {
            std::cerr << "Error: Empty instruction." << std::endl;
            return;
        }

        std::string opcode = tokens[0];
        //  std::cout << opcode.back() << std::endl;
        std::string rd;
        if (tokens.size() > 1)
        {
            rd = tokens[1];
        }

        if (opcode.back() == ':')
        {
        }
        else if (opcode == "lw")
        {
            // std::  cout<<"e"<<endl;
            std::string baseRegister;
            std::string offset;
            int x;
            if (labelToAddress.find(tokens[2]) != labelToAddress.end())
            {
                // reg[rd]=labelToAddress[tokens[2]];
                offset = "0";
                x = labelToAddress[tokens[2]];
            }
            else
            {
                std::string offsetBase = tokens[2];
                size_t openParen = offsetBase.find_first_of('(');
                if (openParen == std::string::npos)
                {
                    std::cerr << "Error: Invalid offset(base) format." << std::endl;
                    return;
                }
                x = reg[offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2)];
                offset = offsetBase.substr(0, openParen);
            }
            int index = 0;

            int s = 0;
            for (int i = 0; i < 4; i++)
            {

                int c = RAM[stoi(offset) + x + i];
                for (int j = 0; j < 8; j++)
                {
                    if (i == 3 && j == 7)
                    {
                        if (c >> 7 & 1 == 1)
                            s -= pow(2, 31);
                        continue;
                    }
                    if (c >> j & 1 == 1)
                        s += pow(2, 8 * i + j);
                }
            }
            reg[rd] = s;
        }

        else if (opcode == "sw")
        {
            std::string offsetBase = tokens[2];
            std::size_t openParen = offsetBase.find_first_of('(');
            if (openParen == std::string::npos)
            {
                std::cerr << "Error: Invalid offset(base) format." << std::endl;
                return;
            }
            std::string baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);
            std::string offset = offsetBase.substr(0, openParen);
            // int index = reg[baseRegister] + stoi(offset)/4; RAM[index] = reg[rd];
            int s = reg[rd];
            for (int i = 0; i < 4; i++)
            {
                int t = 0;
                for (int j = 0; j < 8; j++)
                {
                    if ((s >> (8 * i + j) & 1 == 1))
                        t += pow(2, j);
                }
                RAM[std::stoi(offset) + reg[baseRegister] + i] = t;
            }
        }
        else if (opcode == "la")
        {
            if (labelToAddress.find(tokens[2]) != labelToAddress.end())
            {
                reg[rd] = labelToAddress[tokens[2]];
            }
        }

        else if (opcode == "li")
        {
            int iv = stoi(tokens[2]);
            reg[rd] = iv;
        }
        else if (opcode == "slli")
        {
            std::string rs1 = tokens[2];
            int b = stoi(tokens[3]);
            reg[rd] = reg[rs1] << b;
        }
        else if (opcode == "bne" || opcode == "blt" || opcode == "bgt" || opcode == "beq" || opcode == "bge")
        {
            control_executions(opcode, tokens);
        }
        else if (opcode == "jalr")
        {
            reg[rd] = PC + 1;
            PC = reg[tokens[2]];
        }
        else if (opcode == "j")
        {
            // Jump to the label
            PC = findLabelIndex(tokens[1]);
        }
        else if (opcode == "add" || opcode == "sub" || opcode == "addi")
        {
            // Arithmetic instructions
            executeArithmeticInstruction(tokens);
        }
        else
        {
            std::cerr << "Error: Unknown opcode." << std::endl;
            //  std::cout << opcode << std::endl;
        }

        PC += 1;
    }

    void control_executions(std::string s, std::vector<std::string> tokens)
    {
        std::string rs1 = tokens[1];
        std::string rs2 = tokens[2];
        if (s == "bne")
        {
            if (reg[rs1] != reg[rs2])
            {
                PC = findLabelIndex(tokens[3]);
            }
        }
        else if (s == "blt")
        {
            if (reg[rs1] < reg[rs2])
            {
                PC = findLabelIndex(tokens[3]);
            }
        }
        else if (s == "bgt")
        {
            if (reg[rs1] > reg[rs2])
            {
                PC = findLabelIndex(tokens[3]);
            }
        }
        else if (s == "beq")
        {
            if (reg[rs1] == reg[rs2])
            {
                PC = findLabelIndex(tokens[3]);
            }
        }
        else if (s == "bge")
        {
            if (reg[rs1] >= reg[rs2])
            {
                PC = findLabelIndex(tokens[3]);
            }
        }
    }

    void executeArithmeticInstruction(const std::vector<std::string> &tokens)
    {
        std::string opcode = tokens[0];
        std::string rd = tokens[1];

        if (opcode == "add")
        {
            int op1 = reg[tokens[2]];
            int op2 = reg[tokens[3]];
            reg[rd] = op1 + op2;
        }
        else if (opcode == "sub")
        {
            int op1 = reg[tokens[2]];
            int op2 = reg[tokens[3]];
            reg[rd] = op1 - op2;
        }
        else if (opcode == "addi")
        {
            int op1 = reg[tokens[2]];
            int op2 = stoi(tokens[3]);
            reg[rd] = op1 + op2;
        }
    }
};

class Processor
{
public:
    char RAM[4096];
    int clock = 0;
    bool visited[4096] = {0};
    void run()
    {
        std::cout << "Sorted Array by bubble sort" << std::endl;
        std::ifstream instructionsFile("bubble_sort.txt");
        if (!instructionsFile.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }

        Core core1(instructionsFile);
        core1.readInstructionsFromFile("bubble_sort.txt", RAM, visited);
        core1.executeInstructions(RAM);

        instructionsFile.close();
        std::ifstream instructionsFilee("selection.txt");
        if (!instructionsFilee.is_open())
        {
            std::cerr << "Error opening file." << std::endl;
            return;
        }
        std::cout << "Sorted Array by selection sort" << std::endl;
        Core core2(instructionsFilee);
        core2.readInstructionsFromFile("selection.txt", RAM, visited);
        core2.executeInstructions(RAM);

        instructionsFilee.close();
    }
};

int main()
{
    Processor processor;
    // cout << "Register values:" << endl;
    processor.run();

    return 0;
}