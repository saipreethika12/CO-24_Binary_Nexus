#ifndef PIPE_WOF_H
#define PIPE_WOF_H

#include <fstream>
#include <string>
#include "PU.h"
#include "cache_simulator.h"

class PIPE_WOF : public Core
{
private:
    std::vector<std::pair<std::string, std::string>> latch_IF;
    std::vector<std::pair<std::string, std::string>> latch_IDRF;
    std::vector<std::pair<std::string, std::string>> latch_EXE;
    std::vector<std::pair<std::string, std::string>> latch_MEM;

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
    int c = 0;
    int y = 0, z = 0;
    bool hit = false;
    bool hit_fetch = false;
    bool miss_fetch = false;
    float hits = 0;
    float accesses = 0;
    bool miss = false;

    std::vector<std::vector<std::string>> pip;
    std::vector<std::string> ins_type1;
    std::vector<std::string> ins_type2;
    std::vector<std::string> ins_type3;
    std::vector<std::string> ins_type4;
    std::vector<std::string> ins_type5;
    std::vector<std::string> ins_type6;

public:
    std::map<std::string, int> latency_map;
    PIPE_WOF() : Core(file)
    {
        pip.resize(100, std::vector<std::string>(1000, " "));
        ins_type1 = {"add", "sub", "and", "or", "slt", "mul"};
        ins_type2 = {"addi", "andi", "ori", "sll", "srl", "slli"};
        ins_type3 = {"bne", "beq", "bge", "bgt", "blt"};
        ins_type4 = {"lw", "sw", "la"};
        ins_type5 = {"j", "jal", "jalr"};
        ins_type6 = {"lui"};
    }
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
    bool predict_branch()
    {
        return false;
    }

    void Fetch(Cache_simulator *sim_cache)

    {
        if (PC < instructions.size())
        {
            accesses++;
            hit_fetch = sim_cache->access(ins_map[PC]);
       
            if (hit_fetch == 1)
                hits++;
            else{
                miss_fetch = true;}
            std::string fetched_instruction = instructions[PC].first;
            if (fetched_instruction.back() == ':')
            {
                PC = PC + 1;
                fetched_instruction = instructions[PC].first;
            }
            latch_IF.push_back({"PC", std::to_string(PC)});
            latch_IF.push_back({"fetched_instruction", fetched_instruction});
            std::cout << fetched_instruction << std::endl;

            PC += 1;
            ins++;
        }
        else
        {
            eof = true;
            std::cout << "Reached eof" << std::endl;
        }
        return;
    }
    void Decode()
    {
        if (!stall_flag && !stall_flag2)
        {
            latch_IDRF = latch_IF;
        }
        std::istringstream iss(search_latch("fetched_instruction", latch_IDRF));
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token)
        {
            tokens.push_back(token);
        }
        if (tokens.empty())
        {
            std::cout << "at line no" << PC << std::endl;
            std::cerr << "Error: Empty instruction." << std::endl;
            return;
        }
        std::string opcode = tokens[0];
        std::cout << opcode << std::endl;
        std::string rd;
        if (tokens.size() > 1)
        {
            rd = tokens[1];
        }

        int rs1num;
        int rs2num;

        if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end())
        {
            std::string rs1 = tokens[2];
            std::string rs2 = tokens[3];
            int rs1_value;
            int rs2_value;
            rs1num = returnIndex(rs1);
            rs2num = returnIndex(rs2);

            if (reg_state[rs1num] == 0 || reg_state[rs1num] == 5)
            {
                rs1_value = reg[rs1num];
                stall_flag = false;
                stall_mul = false;
            }
            else
            {

                stall_flag = true;
                stalls += 1;
                count--;
            }
            if (reg_state[rs2num] == 0 || reg_state[rs2num] == 5)
            {
                rs2_value = reg[rs2num];
                stall_flag2 = false;
                stall_mul = false;
            }
            else
            {
                stall_flag2 = true;

                stalls += 1;
                count--;
            }
            if (!stall_flag && !stall_flag2)
            {
                latch_IDRF.push_back({"Opcode", opcode});
                latch_IDRF.push_back({"rd", rd});
                latch_IDRF.push_back({"rs1", rs1});
                latch_IDRF.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF.push_back({"rs2", rs2});
                latch_IDRF.push_back({"rs2_value", std::to_string(rs2_value)});
            }

            reg_state[returnIndex(rd)] = 2;
        }
        if (std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
        {
            int rs1_value;
            std::string rs1 = tokens[2];
            int rs1num = returnIndex(rs1);
            if (reg_state[rs1num] == 0 || reg_state[rs1num] == 5)
            {
           
                rs1_value = reg[rs1num];
                stall_flag = false;
            }
            else
            {
                stall_flag = true;
                stalls += 1;
                count--;
            }
            if (!stall_flag && !stall_flag2)
            {
                latch_IDRF.push_back({"Opcode", opcode});
                latch_IDRF.push_back({"rd", rd});
                latch_IDRF.push_back({"rs1", rs1});
                latch_IDRF.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF.push_back({"immediate", tokens[3]});
                reg_state[returnIndex(rd)] = 2;
            }
        }
        if (std::find(ins_type3.begin(), ins_type3.end(), opcode) != ins_type3.end())
        {
            branch_flag = true;
            executed_branch = false;
            // stalls += 2;
            std::string rs2 = tokens[2];

            std::string rs1 = tokens[1];
            int rs1_value;
            int rs2_value;
            rs1num = returnIndex(rs1);
            rs2num = returnIndex(rs2);
            if (reg_state[rs1num] == 0 || reg_state[rs1num] == 5)
            {
               
                rs1_value = reg[rs1num];
                stall_flag = false;
            }
            else
            {

                std::cout << "stall" << std::endl;
                stall_flag = true;
                stalls += 1;
                count--;
            }
            if (reg_state[rs2num] == 0 || reg_state[rs2num] == 5)
            {
              
                rs2_value = reg[rs2num];
                stall_flag2 = false;
            }
            else
            {
                stall_flag2 = true;
                stalls += 1;
                count--;
            }
            if (!stall_flag && !stall_flag2)
            {
                latch_IDRF.push_back({"Opcode", opcode});
                latch_IDRF.push_back({"rs1", rd});
                latch_IDRF.push_back({"rs2", rs2});
                latch_IDRF.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF.push_back({"rs2_value", std::to_string(rs2_value)});
                latch_IDRF.push_back({"Label", tokens[3]});
            }
        }
        if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())
        {
           
            std::string offset;
            int baseRegister_value;
            int rs1num;
            std::string offsetBase = tokens[2];
            size_t openParen = offsetBase.find_first_of('(');
            std::string baseRegister;
            if (opcode == "lw" && openParen == std::string::npos)
            {
                if (labelToAddress.find(tokens[2]) != labelToAddress.end())
                {
                    rs1num = labelToAddress[tokens[2]];
                    latch_IDRF.push_back({"Opcode", opcode});
                    latch_IDRF.push_back({"rd", rd});
                    latch_IDRF.push_back({"Label", tokens[2]});
                    //  return;
                }
            }
            baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);

            rs1num = returnIndex(baseRegister);

            offset = offsetBase.substr(0, openParen);
            int rdnum = returnIndex(rd);
            int rd_value;
            if (opcode == "lw" && !(openParen == std::string::npos))
            {
              
                if (reg_state[rs1num] == 0 || reg_state[rs1num] == 5)
                {
                    baseRegister_value = reg[rs1num];
                    stall_flag = false; // stall_mul
                }
                else
                {
                    stall_flag = true;
                    stalls += 1;
                    count--; // stall_mul
                }
                if (!stall_flag && !stall_flag2)
                {
                    latch_IDRF.push_back({"Opcode", opcode});
                    latch_IDRF.push_back({"rd", rd});
                    latch_IDRF.push_back({"baseReg_value", std::to_string(baseRegister_value)});
                    latch_IDRF.push_back({"baseRegister", baseRegister});
                    latch_IDRF.push_back({"Offset", offset});
                    reg_state[rdnum] = 2;
                }
                if (reg_state[rdnum] == 0 || reg_state[rdnum] == 5)
                {
                    stall_flag = true;
                    stalls += 1;
                    count--;
                }
            }

            else if (opcode == "sw")
            {
                if (reg_state[rdnum] == 0 || reg_state[rdnum] == 5)
                {
                    rd_value = reg[rdnum];
                    stall_flag = false;
                }
                else
                {
                    stall_flag = true;
                    stalls += 1;
                    count--;
                }

                if (reg_state[rs1num] == 0 || reg_state[rs1num] == 5)
                {
                    baseRegister_value = reg[rs1num];
                    stall_flag2 = false;
                }
                else
                {
                    stall_flag2 = true;
                    stalls += 1;
                    count--;
                }
                if (!stall_flag && !stall_flag2)
                {
                    latch_IDRF.push_back({"Opcode", opcode});
                    latch_IDRF.push_back({"baseRegister", baseRegister});
                    latch_IDRF.push_back({"baseReg_value", std::to_string(baseRegister_value)});
                    latch_IDRF.push_back({"Offset", offset});
                    latch_IDRF.push_back({"rd", rd});
                    latch_IDRF.push_back({"rd_val", std::to_string(rd_value)});
                    reg_state[rdnum] = 2;
                }
            }
            else if (opcode == "la")
            {
                latch_IDRF.push_back({"Opcode", opcode});
                latch_IDRF.push_back({"rd", rd});
                latch_IDRF.push_back({"Label", tokens[2]});
            }
        }
        if (std::find(ins_type5.begin(), ins_type5.end(), opcode) != ins_type5.end())
        {
            branch_flag = true;
            stalls += 2;
            executed_branch = false;
            latch_IDRF.push_back({"Opcode", opcode});
            if (opcode == "j" || opcode == "jal")
            {

                latch_IDRF.push_back({"Label", rd});
            }
            if (opcode == "jalr")
            {
                int rs1Num = returnIndex(tokens[2]);
                int rs1_value = reg[rs1Num];
                latch_IDRF.push_back({"rd", rd});
                latch_IDRF.push_back({"rs1", tokens[2]});
                latch_IDRF.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF.push_back({"Offset", tokens[2]});
                reg_state[returnIndex(rd)] = 2;
            }
        }

        if (stall_flag || stall_flag2)
        {
            imbibe_stall();
        }
    }
    int executeArithmeticInstruction(int op1, int op2, std::string opcode)
    {
        int res = 0;
        if (opcode == "add")
        {
            res = op1 + op2;
        }
        else if (opcode == "sub")
        {
            res = op1 - op2;
        }
        else if (opcode == "addi")
        {
            res = (op1 + op2);
        }
        else if (opcode == "mul")
        {
            res = (op1 * op2);
        }
        else if (opcode == "slli")
        {
            res = op1 >> op2;
        }
        return res;
    }

    void Execute(std::vector<std::pair<std::string, std::string>> latch_IDRF)
    {
        latch_EXE = latch_IDRF;
        std::string opcode;
        std::string rd;
        int result = 0;
        int updatedPC = PC;
        for (const auto &pair : latch_IDRF)
        {
            if (pair.first == "Opcode")
            {
                opcode = pair.second;
            }
        }
        int op1 = 0, op2 = 0;
        for (const auto &pair : latch_IDRF)
        {
            if (pair.first == "rs1_value")
            {
                op1 = stoi(pair.second);
            }
        }
        for (const auto &pair : latch_IDRF)
        {
            if (pair.first == "immediate")
            {

                op2 = stoi(pair.second);
             
            }
            else if (pair.first == "rs2_value")
            {

                op2 = stoi(pair.second);
              
            }
        }

        for (const auto &pair : latch_IDRF)
        {
            if (pair.first == "rd")
            {
                rd = pair.second;
            }
        }
        if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end() || std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
        {

            if (opcode == "add" || opcode == "sub" || opcode == "addi" || opcode == "mul")
            {

                result = executeArithmeticInstruction(op1, op2, opcode);
            }
            
            reg_state[returnIndex(rd)] = 3;
        }
        if (std::find(ins_type3.begin(), ins_type3.end(), opcode) != ins_type3.end())
        {
            for (const auto &pair : latch_EXE)
            {
                std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
            }
            std::cout << op1 << " " << op2 << std::endl;
            result = control_executions(opcode, op1, op2);
            std::cout << "EXECUTED BRANCH" << result << std::endl;
            std::string label = search_latch("Label", latch_IDRF);
            std::cout << "branch pred" << predict_branch() << std::endl;
            if (result != predict_branch())
            {
               
                mis_predict = true;
                stalls += 2;
              
            }
           

            branch_flag = false;
        }
        if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())
        {
           
            int baseReg = 0;
            int offset;

            for (const auto &pair : latch_IDRF)
            {
                if (pair.first == "baseReg_value")
                {
                    std::cout << "First: " << std::endl;
                    baseReg = stoi(pair.second);
                }
            }
            for (const auto &pair : latch_IDRF)
            {
                if (pair.first == "Offset")
                {
                    offset = stoi(pair.second);
                }
            }
            result = baseReg + offset;
            for (const auto &pair : latch_IDRF)
            {
                if (pair.first == "rd_value")
                {
                    op2 = stoi(pair.second);
                }
            }
            if (opcode != "la")
            {
                latch_EXE.push_back({"rs1_value", std::to_string(op2)});
            }
            reg_state[returnIndex(rd)] = 3;
        }
        if (std::find(ins_type5.begin(), ins_type5.end(), opcode) != ins_type5.end())
        {

            std::string label = search_latch("Label", latch_IDRF);
            PC = findLabelIndex(label);
            latch_EXE.push_back({"updated_PC", std::to_string(PC)});

            if (opcode == "jalr")
            {
                reg_state[returnIndex(rd)] = 3;
            }
            branch_flag = false;
        }
        if (opcode != "la" || opcode != "j")
        {
            latch_EXE.push_back({"result", std::to_string(result)});
        }
    }
    void Memory(std::vector<std::pair<std::string, std::string>> latch_EXE, char *RAM, Cache_simulator *sim_cache)
    {
        std::string opcode;
        latch_MEM = latch_EXE;
        for (const auto &pair : latch_EXE)
        {
            if (pair.first == "Opcode")
            {

                opcode = pair.second;
            }
        }
        int result;
        std::string rd = search_latch("rd", latch_EXE);
        for (const auto &pair : latch_EXE)
        {
            if (pair.first == "result")
            {
                result = stoi(pair.second);
            }
        }

        if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end() || std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
        {
            latch_MEM = latch_EXE;
            reg_state[returnIndex(rd)] = 4;
        }
        if (std::find(ins_type3.begin(), ins_type3.end(), opcode) != ins_type3.end())
        {
            executed_branch = true;
        }
        if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())

        {
            int loaded_value = 0;

            if (opcode == "lw")
            {
                accesses++;
                hit = sim_cache->access(result);
                if (!hit)
                {
                    miss = true;
                }
                if (hit)
                    hits++;
                for (int i = 0; i < 4; i++)
                {

                    int c = RAM[result + i];
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
                reg_state[returnIndex(rd)] = 4;
            }
            if (opcode == "sw")
            {
                accesses++;
                int load_value;
                for (const auto &pair : latch_EXE)
                {
                    if (pair.first == "rd_value")
                    {
                        load_value = stoi(pair.second);
                    }
                }
            
                hit = sim_cache->access(result);
                if (!hit)
                {
                    miss = true;
                }
                if (hit)
                    hits++;
                for (int i = 0; i < 4; i++)
                {
                    int t = 0;
                    for (int j = 0; j < 8; j++)
                    {
                        if ((load_value >> (8 * i + j) & 1 == 1))
                            t += pow(2, j);
                    }
                    RAM[result + i] = t;
                }
                reg_state[returnIndex(rd)] = 4;
            }
            if (opcode == "la")
            {
               
                std::string lbl;
                for (const auto &pair : latch_EXE)
                {
                    if (pair.first == "Label")
                    {
                        lbl = pair.second;
                    }
                }
                if (labelToAddress.find(lbl) != labelToAddress.end())
                {
                    accesses++;
                    loaded_value = labelToAddress[lbl];
                    hit = sim_cache->access(loaded_value);
                    if (hit)
                        hits++;
                    if (!hit)
                        miss = true;
                }
            }
            latch_MEM.push_back({"loaded_value", std::to_string(loaded_value)});
        }
        if (std::find(ins_type5.begin(), ins_type5.end(), opcode) != ins_type5.end())
        {
            executed_branch = true;
        }
    }

    void WriteBack(std::vector<std::pair<std::string, std::string>> latch_MEM)
    {

        std::string opcode = search_latch("Opcode", latch_MEM);

        std::string rd = search_latch("rd", latch_MEM);
       
        if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end() || std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
        {
            int result = stoi(search_latch("result", latch_MEM));

           
            reg[returnIndex(rd)] = result;
            reg_state[returnIndex(rd)] = 5;
        }
        if (std::find(ins_type3.begin(), ins_type3.end(), opcode) != ins_type3.end())
        {
        }
        if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())
        {
            if (opcode == "lw")
            {
                std::string loaded_value = search_latch("loaded_value", latch_MEM);
                reg[returnIndex(rd)] = stoi(loaded_value);
                reg_state[returnIndex(rd)] = 5;
            }
            if (opcode == "sw")
            {
            }
            if (opcode == "la")
            {
                std::string loaded_value = search_latch("loaded_value", latch_MEM);
                reg[returnIndex(rd)] = stoi(loaded_value);
                reg_state[returnIndex(rd)] = 5;
             
            }

            reg_state[returnIndex(rd)] = 5;
        }
        if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())
        {
            if (opcode == "jalr")
            {
                reg[returnIndex(rd)] = PC + 1;
             
            }
        }
    }
    void Step_count(char *RAM, Cache_simulator *sim_cache)
    {
        int latency_addi = latency_map["ADDI"] - 1;
        int latency_add = latency_map["ADD"] - 1;
        int latency_mul = latency_map["MUL"] - 1;
        int latency_sub = latency_map["SUB"] - 1;
        int mem_access_latency = sim_cache->get_mem_latency() - 1;
        int cache_latency = sim_cache->get_cache_latency() - 1;
         int mem_access_latency_f = sim_cache->get_mem_latency() - 1;
        int cache_latency_f = sim_cache->get_cache_latency() - 1;

        while (keep_going)
        {
           
            c++;
            y = 0;
            int k = 0;
            bool cont = false;
         
            if (latch_MEM.size() > 0 && (!hit) && !miss)
            {
                k = 1;
                std::cout << "WB";
                //  pip[y + z][c] = "W";
                z++;
                for (const auto &pair : latch_MEM)
                {
                    std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
                }
          
                   

                WriteBack(latch_MEM);
             
                latch_MEM.clear();
            }
            if (latch_EXE.size() > 0)
            {

                k = 2;
                std::cout << "M";
                // pip[y + z][c] = "M";
                y++;

                if (!hit && !miss)
                {
                    Memory(latch_EXE, RAM, sim_cache);
                }
                std::string opcode = search_latch("Opcode", latch_EXE);
                if (opcode == "lw" || opcode == "la" || opcode == "sw")
                {
                    if (miss)
                    {
                        if (mem_access_latency > 0)
                        {
                            
                            mem_access_latency--;
                            loop++;
                            stalls++;
                            cont = true;
                        }
                    }
                    else if (hit)
                    {
                        if (cache_latency > 0)
                        {
                            cache_latency--;
                            loop++;
                            stalls++;
                            cont = true;
                        }
                    }
                }
                if (cont == true)
                {
                    continue;
                }

                if (executed_branch && mis_predict)
                {
                    
                    PC = (findLabelIndex(search_latch("Label", latch_MEM)));
                    latch_IF.clear();
                    std::string rd = search_latch("rd", latch_IDRF);
                  
                    reg_state[returnIndex(rd)] = 0;
                
                    latch_IDRF.clear();
                    executed_branch = false;
                    mis_predict = false;
                }

                if (mem_access_latency == 0)
                {
                    mem_access_latency = sim_cache->get_mem_latency() - 1;
                    miss = false;
                }
                if (cache_latency == 0)
                {
                    cache_latency = sim_cache->get_cache_latency() - 1;
                    hit = false;
                }
                latch_EXE.clear();
            }
            if (latch_IDRF.size() > 0)
            {

                if (!stall_flag && !stall_flag2)
                {
                    bool f = false;
                    k = 3;
                    std::cout << "E";
                    std::string op = search_latch("Opcode", latch_IDRF);

                    // pip[y + z][c] = "E";
                    y++;
                    if (op == "addi")
                    {
                        if (latency_addi > 0)
                        {
                       
                            latency_addi--;
                            loop++;
                            continue;
                        }
                    }
                    else if (op == "add")
                    {
                        if (latency_add > 0)
                        {
                            f = true;
                            latency_add--;
                            loop++;
                            continue;
                        }
                    }
                    else if (op == "sub")
                    {
                        if (latency_sub > 0)
                        {
                            f = true;
                            latency_sub--;
                            loop++;
                            continue;
                        }
                    }
                    else if (op == "mul")
                    {
                        if (latency_mul > 0)
                        {
                            f = true;
                            latency_mul--;
                            loop++;
                            continue;
                        }
                    }
                    if (f)
                    {   stalls++;
                        continue;
                    }
                    Execute(latch_IDRF);
                    latency_addi = latency_map["ADDI"] - 1;
                    latency_add = latency_map["ADD"] - 1;
                    latency_mul = latency_map["MUL"] - 1;
                    latency_sub = latency_map["SUB"] - 1;
                    latch_IDRF.clear();
                }
            }
            if(!miss_fetch && !hit_fetch){
            if (latch_IF.size() > 0 ||( stall_flag || stall_flag2))
            {
                if (latch_IDRF.size() == 0)
                {
                    k = 4;
                    // pip[y + z][c] = "D";
                    y++;
                    Decode();

                    std::cout << "ID";
                    latch_IF.clear();
                }
                else if ((stall_flag || stall_flag2) && latch_IDRF.size() == 2)
                {
                    k = 4;
                    //

                    // pip[y + z][c] = "D";
                    y++;
                    Decode();

                    std::cout << "ID";
                    //  latch_IF.clear();
                }
                
            }
            }  
            if ((latch_IF.size() == 0 && !eof) || (latch_IF.size()==2 &&(miss_fetch || hit_fetch)))
            {
                k = 5;
                if(!miss_fetch && !hit_fetch){
                Fetch(sim_cache);
                }
                if (miss_fetch)
                {
                    if (mem_access_latency_f > 0)
                    {
                        
                        mem_access_latency_f--;
                        loop++;
                        stalls++;
                        cont = true;
                    }
                }
                else if (hit_fetch)
                {
                    if (cache_latency_f > 0)
                    {
                        cache_latency_f--;
                        loop++;
                        stalls++;
                        cont = true;
                    }
                }
                if (cont == true)
                {
                    continue;
                }
                if (mem_access_latency_f == 0)
                {
                    mem_access_latency_f = sim_cache->get_mem_latency() - 1;
                    miss_fetch = false;
                }
                if (cache_latency_f == 0)
                {
                    cache_latency_f = sim_cache->get_cache_latency() - 1;
                    hit_fetch = false;
                }

                std::cout << "F";
            }
            else if(eof && executed_branch && !mis_predict)
            {
             break;
               
            }
          
            if (k == 0)
            { 
                keep_going = 0;
                break;
               
            }
            else
            {
                loop++;
                std::cout << " "
                          << "cycles" << loop << std::endl;
            }
            std::cout << "\nONE cycle finish\n";
        
            
        }
        std::cout << "No of stalls" << std::endl;
        std::cout << stalls << std::endl;
        std::cout << "No of cycles" << std::endl;
        std::cout << loop << std::endl;
        std::cout << "Instructions per cycle" << std::endl;
        std::cout << (float)ins / loop << std::endl;
        std::cout << "Hit Rate" << std::endl;
        std::cout << hits / accesses << std::endl;
        // for (int i = 0; i <30; i++)
        // {
        //     for (int j = 0; j < 30; j++)
        //     {
        //         std::cout << pip[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }
};
#endif