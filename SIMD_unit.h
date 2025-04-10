#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint> 
#include <ctime>
#include <bits/stdc++.h>
#ifndef SIMD_H
#define SIMD_H
class SIMD_unit {
public:
    std::unordered_map<std::string, long long int> labelToAddress;
    std::map<std::string, std::pair<int, int>> labelInfo;
    long long int reg[32];
    int reg_state[32] = {0};
    int PC;
    std::vector<std::pair<std::string, int>> instructions;
    std::ifstream file;
    int start_of_ins;
    std::map<int, int> ins_map;


public:
    // Constructor that accepts filename, start_of_ins, RAM, and vis
    // SIMD_unit(const std::string& filename, int start_ins, char* RAM, bool* vis) : start_of_ins(start_ins) {
    //     file.open(filename);
    //     if (!file.is_open()) {
    //         std::cerr << "Error opening file." << std::endl;
    //         return;
    //     }

    //     // Call function to parse the input file
    //     readInstructionsFromFile(RAM, vis);
    // }
    SIMD_unit(){
        PC=0;
    }

    // Function to read instructions from the file
void readInstructionsFromFile(const std::string& filename,char* RAM, bool* vis) {
    std::ifstream file;
      file.open(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
            return;
        }
    int index = start_of_ins;
    std::string line;
    bool inDataSection = false;
    std::string currentLabel;

    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty())
            continue;

        // Check if the line indicates a section change
        if (line == ".data") {
            inDataSection = true;
            continue;
        } else if (line == ".text") {
            inDataSection = false;
            continue;
        }

        // Process lines based on section
        if (inDataSection) {
            // Data section: Parse labels and data
            std::istringstream iss(line);
           // std::cout<<line<<std::endl;
            std::string label;
            iss >> label;

            if (label.back() == ':') {
                // Remove ':' from label
                label.pop_back();

                currentLabel = label;

                long long int address = get_index(vis);
                labelToAddress[label] = address;
                  

                int length = 0;
                int add_temp = address;
                std::string directive;
                while (iss >> directive) {
                    if (directive == ".len") {
                        iss>>length;
                        //break;
                    }
                      if (directive == ".word")
                            continue;
                        if (isInteger(directive))
                        {
                          //  std::cout<<"addre"<<add_temp<<std::endl;
                            int val = std::stoi(directive);
                           // std::cout<<" value "<<val<<std::endl;
                            for (int i = 0; i < 4; i++)
                            {
                                int t = 0;
                                for (int j = 0; j < 8; j++)
                                {
                                    if ((val >> (8 * i + j) & 1 == 1))
                                        t += pow(2, j);
                                }
                                RAM[add_temp] = t;
                                vis[add_temp] = 1;
                                add_temp++;
                            }
                            int lv=0;
                               for (int i = 0; i < 4; i++)
                {
                  
                    int c = RAM[41 + i];
                    for (int j = 0; j < 8; j++)
                    {
                        if (i == 3 && j == 7)
                        {
                            if (c >> 7 & 1 == 1)
                                lv -= pow(2, 31);
                            continue;
                        }
                        if (c >> j & 1 == 1)
                            lv += pow(2, 8 * i + j);
                    }

                }
               // std::cout<<lv<<std::endl;
                        }
                }
                  labelInfo[label] = std::make_pair(address, length);
                //std::cout<<label<<std::endl;
             
            

               // while (iss >> directive) {
                    // if (directive == ".word") {
                    //     std::string value;
                    //     for (int i = 0; i < length; i++) {
    
                    //         if (iss >> value) {
                    //                int val = std::stoi(value);
                    //         for (int i = 0; i < 4; i++)
                    //         {
                    //             int t = 0;
                    //             for (int j = 0; j < 8; j++)
                    //             {
                    //                 if ((val >> (8 * i + j) & 1 == 1))
                    //                     t += pow(2, j);
                    //             }
                    //             RAM[address] = t;
                    //             vis[address] = 1;
                    //             address++;
                    //         }
                    //         } else {
                    //             std::cerr << "Error parsing data." << std::endl;
                    //             break;
                    //         }
                    //     }
                    // }
                  
                        // if (directive == ".word")
                        //     continue;
                        // if (isInteger(directive))
                        // {
                        //     int val = std::stoi(directive);
                        //     for (int i = 0; i < 4; i++)
                        //     {
                        //         int t = 0;
                        //         for (int j = 0; j < 8; j++)
                        //         {
                        //             if ((val >> (8 * i + j) & 1 == 1))
                        //                 t += pow(2, j);
                        //         }
                        //         RAM[address] = t;
                        //         vis[address] = 1;
                        //         address++;
                        //     }
                        // }
                    
               // }
            }
        } else {
            // Text section: Process instructions
             // std::cout<<"add ins"<<std::endl;
            instructions.push_back(std::make_pair(line, index));
            ins_map[index] = index;


            // Store instruction in RAM (assuming 4 bytes per instruction)
            // for (int i = 0; i < 4; i++) {
            //     int t = 0;
            //     for (int j = 0; j < 8; j++) {
            //         if ((index >> (8 * i + j) & 1) == 1)
            //             t += pow(2, j);
            //     }
            //     RAM[index] = t;
            //     vis[index] = 1;
            //     index++;
            // }
        }
    }
    //  int store = 0;
    //     for (int i = 0; i < 1024; i++){
            
    //          int lv =0;
    //                for (int i = 0; i < 4; i++)
    //             {
                  
    //                 int c = RAM[store + i];
    //                 for (int j = 0; j < 8; j++)
    //                 {
    //                     if (i == 3 && j == 7)
    //                     {
    //                         if (c >> 7 & 1 == 1)
    //                             lv -= pow(2, 31);
    //                         continue;
    //                     }
    //                     if (c >> j & 1 == 1)
    //                         lv += pow(2, 8 * i + j);
    //                 }

    //             }
    //             store+=4;
    //             std::cout<<i<<" "<<lv<<std::endl;
    //     }

    file.close();
}


    // Function to generate a random index for data storage
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
    void print_ram(char*RAM){
        // int store = 0;
        // for (int i = 0; i < 1024; i++){
            
        //      int lv =0;
        //            for (int i = 0; i < 4; i++)
        //         {
                  
        //             int c = RAM[store + i];
        //             for (int j = 0; j < 8; j++)
        //             {
        //                 if (i == 3 && j == 7)
        //                 {
        //                     if (c >> 7 & 1 == 1)
        //                         lv -= pow(2, 31);
        //                     continue;
        //                 }
        //                 if (c >> j & 1 == 1)
        //                     lv += pow(2, 8 * i + j);
        //             }

        //         }
        //         store+=4;
        //         std::cout<<i<<" "<<lv<<std::endl;
        // }
    }
    void execute(char*RAM){
        // std::cout <<"is "<<instructions.size()<<std::endl;
        // std::cout<<PC<<std::endl;
        while(PC<instructions.size())
        {
            std::string ins = instructions[PC].first;
            //std::cout<<ins<<std::endl;
              std::istringstream iss(ins);
              std::string token;
              std::vector<std::string>tokens;
              while(iss >>token){
                tokens.push_back(token);
              }
              std:: string opcode=tokens[0];
              int store = labelInfo[tokens[1]].first;
             // std::cout<<"store"<<store<<std::endl;
              int lv=0;
                     for (int i = 0; i < 4; i++)
                {
                   
                    int c = RAM[store + i];
                    for (int j = 0; j < 8; j++)
                    {
                        if (i == 3 && j == 7)
                        {
                            if (c >> 7 & 1 == 1)
                                lv -= pow(2, 31);
                            continue;
                        }
                        if (c >> j & 1 == 1)
                            lv += pow(2, 8 * i + j);
                    }

                }
                int store_address = lv;
                int sat = lv;
               // std::cout<<"   store add" <<lv<<std::endl;
              int base_address = (labelInfo[tokens[2]].first);
             // std::cout<<"ba"<<base_address<<std::endl;
              int size = (labelInfo[tokens[2]].second);
               int base_address_v2 = (labelInfo[tokens[3]].first);
               // std::cout<<"ba2"<<base_address_v2<<std::endl;
              int size_v2 = (labelInfo[tokens[3]].second);
              // std::cout<<"si2"<<size_v2<<std::endl;
              std::vector<int>v1,v2;
              for(int k=0;k<size;k++){
                 long long int loaded_value=0;
                      for (int i = 0; i < 4; i++)
                {
                   
                    int c = RAM[base_address + i];
                    for (int j = 0; j < 8; j++)
                    {
                        if (i == 3 && j == 7)
                        {
                            if (c >> 7 & 1 == 1)
                                loaded_value -= pow(2, 31);
                            continue;
                        }
                        if (c >> j & 1 == 1)
                            loaded_value += pow(2, 8 * i + j);
                    }

                }
               // std::cout<<"loaded"<<loaded_value<<std::endl;
                base_address+=4;
                v1.push_back(loaded_value);
              }
               
                for(int k=0;k<size;k++){
                 long long int loaded_value=0;
                      for (int i = 0; i < 4; i++)
                {
                   
                    int c = RAM[base_address_v2 + i];
                    for (int j = 0; j < 8; j++)
                    {
                        if (i == 3 && j == 7)
                        {
                            if (c >> 7 & 1 == 1)
                                loaded_value -= pow(2, 31);
                            continue;
                        }
                        if (c >> j & 1 == 1)
                            loaded_value += pow(2, 8 * i + j);
                    }
                   
                }
                 
                base_address_v2+=4;
                v2.push_back(loaded_value);
              }
              if(opcode == "add_vec"){
               // std::cout<<"add ins"<<std::endl;
                      for(int i=0;i<size;i++){
                        int sv = v1[i]+v2[i];
                        for (int i = 0; i < 4; i++) {
                            int t = 0;
                            for (int j = 0; j < 8; j++) {
                                if ((sv >> (8 * i + j) & 1) == 1)
                                    t += pow(2, j);
                            }
                            RAM[store_address] = t;
                            //vis[store_address] = 1;
                            store_address++;
                            }
                      }
              }
              else if(opcode == "sub_vec"){
                 // std::cout<<"sub ins"<<std::endl;
                for(int i=0;i<size;i++){
                        int sv = v1[i]-v2[i];
                        for (int i = 0; i < 4; i++) {
                            int t = 0;
                            for (int j = 0; j < 8; j++) {
                                if ((sv >> (8 * i + j) & 1) == 1)
                                    t += pow(2, j);
                            }
                            RAM[store_address] = t;
                            //vis[store_address] = 1;
                            store_address++;
                            }
                      }
              }
                 else if(opcode == "mul_vec"){
                     // std::cout<<"mul ins"<<std::endl;
                for(int i=0;i<size;i++){
                        int sv = v1[i]*v2[i];
                        for (int i = 0; i < 4; i++) {
                            int t = 0;
                            for (int j = 0; j < 8; j++) {
                                if ((sv >> (8 * i + j) & 1) == 1)
                                    t += pow(2, j);
                            }
                            RAM[store_address] = t;
                            //std::cout<<t<<std::endl;
                            //vis[store_address] = 1;
                            store_address++;
                            }
                      }
              }
              else {
                std::cout<<"Invalid Opcode"<<std::endl;
              }
           PC+=1; 
            printResult(RAM,size,sat);
        } 
        // long long int loaded_value=0;
        //         for (int i = 0; i < 4; i++)
        //         {
                   
        //             int c = RAM[400+ i];
        //             for (int j = 0; j < 8; j++)
        //             {
        //                 if (i == 3 && j == 7)
        //                 {
        //                     if (c >> 7 & 1 == 1)
        //                         loaded_value -= pow(2, 31);
        //                     continue;
        //                 }
        //                 if (c >> j & 1 == 1)
        //                     loaded_value += pow(2, 8 * i + j);
        //             }

             //   }

              
              
    }
    void printResult(char* RAM,int len, int base_adddress){
            std::cout<<"The values from base address  "<<base_adddress<<std::endl;
               for(int k=0;k<len*4; k+=4){
                        long long int loaded_value=0;
               int b_add=base_adddress+k;
                for (int i = 0; i < 4; i++)
                {
                   
                    int c = RAM[b_add+ i];
                    for (int j = 0; j < 8; j++)
                    {
                        if (i == 3 && j == 7)
                        {
                            if (c >> 7 & 1 == 1)
                                loaded_value -= pow(2, 31);
                            continue;
                        }
                        if (c >> j & 1 == 1)
                            loaded_value += pow(2, 8 * i + j);
                    }

                }
                std::cout<<loaded_value<<" "<<std::endl;
                }
                std::cout<<std::endl;
    }

};
#endif