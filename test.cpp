#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

void parseRiscVInstruction(const string& instruction, uint32_t& pc, unordered_map<string, int>& registers, unordered_map<string, int>& labels) {
    // Tokenize the instruction
    istringstream iss(instruction);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    // Ensure tokens vector contains at least 4 elements
    if (tokens.size() < 4) {
        cerr << "Error: Invalid instruction format." << endl;
        return;
    }

    // Extract opcode
    string opcode = tokens[0];

    // Handle different opcodes
    if (opcode == "lw" || opcode == "sw") {
        // Load or Store instruction
        string rd = tokens[1]; // Destination register

        // Extract base register and offset
        string offsetBase = tokens[3];
        size_t openParen = offsetBase.find_first_of('(');
        if (openParen == string::npos) {
            cerr << "Error: Invalid offset(base) format." << endl;
            return;
        }
        string baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);
        string offset = offsetBase.substr(0, openParen);

        // Store register values
        registers[rd] =  registers[baseRegister] +stoi(offset); // Initialize destination register value to 0
        // Initialize base register value to 0

        cout << "Opcode: " << opcode << ", Destination Register: " << rd << ", Base Register: " << baseRegister << ", Offset: " << offset << endl;
    } else {
        // Other instructions
        // Handle different types of instructions as needed
        cout << "Opcode: " << opcode << endl;
    }

    // Increment PC
    pc += 4; // Assuming each instruction is 4 bytes long
}


int main() {
    // File path
    string filePath = "instuctions.txt"; // Change this to your file path
     int k=stoi("-3");
     cout<<k;
    // Open the file
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    // Initialize program counter
    uint32_t pc = 0;

    // Map to store registers and their values
    unordered_map<string, int> registers;

    // Initialize registers x1, x2, x3, ...
    for (int i = 1; i <= 31; ++i) {
        registers["x" + to_string(i)] = 0;
    }

    // Map to store labels and their addresses
    unordered_map<string, int> labels;

    // Read the file line by line
    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Check if the line contains a label
        if (line.back() == ':') {
            // Extract label
            string label = line.substr(0, line.size() - 1);
            // Store label with its address
            labels[label] = pc;
        } else {
            // Parse instruction
            parseRiscVInstruction(line, pc, registers, labels);
        }
    }

    // Close the file
    file.close();

    // Print register values
    cout << "Register values:" << endl;
   for (auto it = registers.begin(); it != registers.end(); ++it) {
        cout << it->first << ": " << it->second << endl;
    }

    return 0;
}
