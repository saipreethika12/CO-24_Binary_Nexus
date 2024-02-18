#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

int main() {
    // File path
    string filePath = "instuctions.txt"; // Change this to your file path

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
