#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <bits/stdc++.h>
using namespace std;

class Core {
private:
 map<string, long long int> labelToAddress;
    map<string, int> reg;
    int PC;
    vector<pair<string, int>> instructions;
    ifstream& file;

public:
    Core(ifstream& file) : file(file), PC(0) {
        for (int i = 0; i <= 31; ++i) {
            reg["x" + to_string(i)] = 0;
        }
    }
   void readInstructionsFromFile(const string& filename, int* RAM) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    int index = 0;
    string line;
    bool isDataSection = false;

    while (getline(file, line)) {
        if (line == ".data") {
            isDataSection = true;
            continue;
        } else if (isDataSection) {
            istringstream iss(line);
            string label;
            iss >> label;
                   if (label.back() == ':') {
                    // It's an array declaration
                    label.pop_back(); // Remove the colon from the label
                   
                    // Randomize the index for array declaration
                    long long int address = rand() % 4096; // Adjust as needed
                    labelToAddress[label] = address;
                     string value;
                   while(iss >> value) {
                    try {
                        int intValue = stoi(value);
                        RAM[address++] = intValue;
                    } catch (const exception& e) {
                        cerr << "Error converting string to integer: " << e.what() << endl;
                        cerr << "Problematic string: '" << value << "'" << endl;
                        // Handle the error as needed
                    }
                }

                    // string value;
                    // while (iss >> value) {
                    //     int k=stoi(value);
                    //     RAM[address++] = k;
                 }
            // if (label.back() == ':') {
            //     // It's an array declaration
            //     label.pop_back(); // Remove the colon from the label
            //     int value;
            //     while (iss >> value) {
            //         RAM[index++] = value;
            //     }

            //     // Store the address of the array in a register (you may need to adjust this based on your design)
            //     reg[label] = ;
            else {
                // Not an array declaration, exit the data section
                isDataSection = false;
            }
             
        } else {
            instructions.push_back(make_pair(line, index));
            index++;
        }
    }

    file.close();
}


    int findLabelIndex(const string& label) {
        for (const auto& instruction : instructions) {
            istringstream iss(instruction.first);
            vector<string> tokens;
            string token;
            while (iss >> token) {
                tokens.push_back(token);
            }
            if (tokens.size() > 0 && tokens[0] == (label+':')) {
               
                // Return the index of the label
                return instruction.second;
            }
        }
        cerr << "Error: Label not found - " << label << endl;
        return PC; // Default to current PC if label is not found
    }

    void executeInstructions(int* RAM) {
        while (PC < instructions.size()) {
            execute(instructions[PC].first, RAM);
        }

        for (auto it = reg.begin(); it != reg.end(); ++it) {
            cout << it->first << ": " << it->second << endl;
        }
         for (int i = 0; i < 4096; ++i) {
            cout << "RAM[" << i << "]: " << RAM[i] << endl;
        }
    }

private:
    void execute(const string& instruction, int* RAM) {
        istringstream iss(instruction);
        vector<string> tokens;
        string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            cerr << "Error: Empty instruction." << endl;
            return;
        }

        string opcode = tokens[0];
        string rd = tokens[1];

        if (opcode == "lw") {
            // Load instruction
            string offsetBase = tokens[2];
            size_t openParen = offsetBase.find_first_of('(');
            if (openParen == string::npos) {
                cerr << "Error: Invalid offset(base) format." << endl;
                return;
            }

            string baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);
            string offset = offsetBase.substr(0, openParen);

            int index = reg[baseRegister] + stoi(offset);
            reg[rd] = RAM[index];  // Load value from RAM
        }else if (opcode == "sw") {
            // Store instruction
            string offsetBase = tokens[2];
            size_t openParen = offsetBase.find_first_of('(');
            if (openParen == string::npos) {
                cerr << "Error: Invalid offset(base) format." << endl;
                return;
            }

            string baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);
            string offset = offsetBase.substr(0, openParen);

            int index = reg[baseRegister] + stoi(offset);
            RAM[index] = reg[rd];  // Store value to RAM
        } else if (opcode == "add" || opcode == "sub" || opcode == "addi") {
            // Arithmetic instructions
            executeArithmeticInstruction(tokens);
        } else if (opcode == "jalr") {
            reg[rd] = PC + 1;
            PC = reg[tokens[2]];

        } else if (opcode == "jal") {
            // Jump to the label
            PC = findLabelIndex(tokens[2]);

        } else if (opcode == "slli") {
            // Handle slli instruction
            // ...

        } else if (opcode == "bne" || opcode == "blt" || opcode == "bgt" || opcode == "beq") {
            control_executions(opcode, tokens);
        } else {
            // Unknown opcode
            cerr << "Error: Unknown opcode." << endl;
        }

        // Increment PC
        PC += 1; // Increment PC to point to the next instruction
    }

    void control_executions(string s, vector<string> tokens) {
        string rs1 = tokens[1];
        string rs2 = tokens[2];
        if (s == "bne") {
            if (reg[rs1] != reg[rs2]) {
                PC = findLabelIndex(tokens[3]);
            }
        }
        if (s == "blt") {
            if (reg[rs1] < reg[rs2]) {
                PC = findLabelIndex(tokens[3]);
            }
        }
        if (s == "bgt") {
            if (reg[rs1] > reg[rs2]) {
                PC = findLabelIndex(tokens[3]);
            }
        }
        if (s == "beq") {
            if (reg[rs1] == reg[rs2]) {
                PC = findLabelIndex(tokens[3]);
            }
        }
    }


    void executeArithmeticInstruction(const vector<string>& tokens) {
        string opcode = tokens[0];
        string rd = tokens[1];

        if (opcode == "add") {
            // Addition instruction
            int op1 = reg[tokens[2]];
            int op2 = reg[tokens[3]];
            reg[rd] = op1 + op2;
        } else if (opcode == "sub") {
            // Subtraction instruction
            int op1 = reg[tokens[2]];
            int op2 = reg[tokens[3]];
            reg[rd] = op1 - op2;
        } else if (opcode == "addi") {
            // Add immediate instruction
            int op1 = reg[tokens[2]];
            int op2 = stoi(tokens[3]);
            reg[rd] = op1 + op2;
        }
    }
};

class Processor {
public:
    int RAM[4096];
    int clock = 0;

    void run() {
        ifstream instructionsFile("instuctions.txt");
        if (!instructionsFile.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        Core core(instructionsFile);
        core.readInstructionsFromFile("instuctions.txt", RAM);
        core.executeInstructions(RAM);

        instructionsFile.close();
    
    }

};

int main() {
    Processor processor;
    processor.run();
    cout << "Register values:" << endl;

    return 0;
}

