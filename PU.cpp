#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Core {
private:
    unordered_map<string, int> reg;
    int PC;
    vector<string> instructions;
    ifstream& file;

public:
    Core(ifstream& file) : file(file), PC(0) {
        for (int i = 0; i <= 31; ++i) {
            reg["x" + to_string(i)] = 0;
        }
    }

    void readInstructionsFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            instructions.push_back(line);
        }

        file.close();
    }

    void executeInstructions(int* RAM) {
        for (const string& instruction : instructions) {
            execute(instruction, RAM);
        }
        file.seekg(PC);
          for (auto it = reg.begin(); it != reg.end(); ++it) {
        cout << it->first << ": " << it->second << endl;
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

        // Extract opcode
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
        } else if (opcode == "sw") {
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
        } else {
            // Unknown opcode
            cerr << "Error: Unknown opcode." << endl;
        }

        // Increment PC
        PC += 4; // Assuming each instruction is 4 bytes long
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
            //    f.open(instructionsFile);
        if (!instructionsFile.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        Core core(instructionsFile);
        core.readInstructionsFromFile("instuctions.txt");
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
