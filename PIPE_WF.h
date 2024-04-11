#ifndef PIPE_WF_H
#define PIPE_WF_H
#include "PU.h"

// std::vector<std::pair<std::string, std::string>> latch_IF;
// std::vector<std::pair<std::string, std::string>> latch_IDRF;
// std::vector<std::pair<std::string, std::string>> latch_EXE;
// std::vector<std::pair<std::string, std::string>> latch_MEM;
// std::map<std::string, int> latency_map;

// // std::vector<std::pair<std::string, std::string>> latch_WB;
// bool ishazard_notified = false;
// bool predict_branch(){
//     return false;
// }
// int stalls = 0;
// bool eof = false;
// int ins = 0;
// int prevPC = 0;
// int prevprevPC = 0;
// bool mis_predict = false;
// void imbibe_stall()
// {
//     std::cout << "STAll"
//               << " ";
// }
// std::string search_latch(const std::string &opcode, const std::vector<std::pair<std::string, std::string>> &latch_IDRF)
// {
//     for (const auto &pair : latch_IDRF)
//     {
//         if (pair.first == opcode)
//         {
//             return pair.second; // Return the value if opcode matches
//         }
//     }
//     return ""; // Return empty string if opcode is not found
// }
// bool isHazard(std::string rs, std::vector<std::pair<std::string, std::string>> v)
// {
//     if (latch_MEM.size() > 0)
//     {
//         std::cout<<"CAME HERE"<<" "<<(rs == search_latch("rd", v))<<std::endl;
//         return rs == search_latch("rd", v);
//     }
//     else
//         return false;
// }
// // #include <vector>
// // #include <map>
// // #include <string>

// // extern std::vector<std::pair<std::string, std::string>> latch_IF;
// // extern std::vector<std::pair<std::string, std::string>> latch_IDRF;
// // extern std::vector<std::pair<std::string, std::string>> latch_EXE;
// // extern std::vector<std::pair<std::string, std::string>> latch_MEM;
// // extern std::map<std::string, int> latency_map;
// // extern bool ishazard_notified;
// // extern bool stall_flag;
// // extern bool stall_flag2;
// // extern bool branch_flag;
// // extern bool stall_mul;
// // extern bool executed_branch;
// // extern int stalls;
// // extern int count;
// // extern bool eof;
// // extern int ins;
// // extern int prevPC;
// // extern int prevprevPC;
// // extern bool mis_predict;

// // extern std::vector<std::string> ins_type1;
// // extern std::vector<std::string> ins_type2;
// // extern std::vector<std::string> ins_type3;
// // extern std::vector<std::string> ins_type4;
// // extern std::vector<std::string> ins_type5;
// // extern std::vector<std::string> ins_type6;
// std::vector<std::string> ins_type1 = {"add", "sub", "and", "or", "slt"};
// std::vector<std::string> ins_type2 = {"addi", "andi", "ori", "sll", "srl", "slli"};
// std::vector<std::string> ins_type3 = {"bne", "beq", "bge", "bgt", "blt"};
// std::vector<std::string> ins_type4 = {"lw", "sw", "la"};
// std::vector<std::string> ins_type5 = {"j", "jal", "jalr"};
// std::vector<std::string> ins_type6 = {"lui"};
// bool stall_flag = false;
// bool stall_flag2 = false;
// bool branch_flag = false;
// bool stall_mul = false;
// bool executed_branch = false;
// int count = 1;
/*
CHANGE EACH OF THE names to _WF to have seperate variables and avoid conflictt
*/
class PIPE_WF : public Core
{
private:
    std::vector<std::pair<std::string, std::string>> latch_IF_wf;
    std::vector<std::pair<std::string, std::string>> latch_IDRF_wf;
    std::vector<std::pair<std::string, std::string>> latch_EXE_wf;
    std::vector<std::pair<std::string, std::string>> latch_MEM_wf;

    bool ishazard_notified_wf = false;
    int stalls_wf = 0;
    bool eof_wf = false;
    int ins_wf = 0;
    int prevPC_wf = 0;
    int prevprevPC_wf = 0;
    bool mis_predict_wf = false;
    bool stall_flag_wf = false;
    bool stall_flag2_wf = false;
    bool branch_flag_wf = false;
    bool stall_mul_wf = false;
    bool executed_branch_wf = false;
    int count_wf = 1;
    int deof_wf = false;
    int loop_wf = 0;
    // bool keep_going_wf = true;
    int v = 100;
    bool hit = false;
    bool miss = false;
    float hits = 0;
    float accesses = 0;
    bool hit_fetch = false;
    bool miss_fetch = false;
    bool j = false;

    std::vector<std::vector<std::string>> pip;
    std::vector<std::string> ins_type1_wf;
    std::vector<std::string> ins_type2_wf;
    std::vector<std::string> ins_type3_wf;
    std::vector<std::string> ins_type4_wf;
    std::vector<std::string> ins_type5_wf;
    std::vector<std::string> ins_type6_wf;

public:
    std::map<std::string, int> latency_map;
    // Constructor
    PIPE_WF() : Core(file)
    {
        pip.resize(100, std::vector<std::string>(1000, " "));
        ins_type1_wf = {"add", "sub", "and", "or", "slt", "mul"};
        ins_type2_wf = {"addi", "andi", "ori", "sll", "srl", "slli"};
        ins_type3_wf = {"bne", "beq", "bge", "bgt", "blt"};
        ins_type4_wf = {"lw", "sw", "la"};
        ins_type5_wf = {"j", "jal", "jalr"};
        ins_type6_wf = {"lui"};
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
    bool isHazard(std::string rs, std::vector<std::pair<std::string, std::string>> v)
    {
        if (latch_MEM_wf.size() > 0)
        {
            std::cout << "CAME HERE"
                      << " " << (rs == search_latch("rd", v)) << std::endl;
            return rs == search_latch("rd", v);
        }
        else
            return false;
    }
    void FetchWF(Cache_simulator *sim_cache)
    {

        if (PC < instructions.size())
        {
            accesses++;
            hit_fetch = sim_cache->access(ins_map[PC]);
            if (hit_fetch == 1)
                hits++;
            else
            {
                miss_fetch = true;
            }
            hit_fetch = sim_cache->access(ins_map[PC]);
            std::string fetched_instruction = instructions[PC].first;
            if (fetched_instruction.back() == ':')
            {
               std::cout << fetched_instruction << std::endl; 
                PC = PC + 1;
                fetched_instruction = instructions[PC].first;
            }
            latch_IF_wf.push_back({"PC", std::to_string(PC)});
            latch_IF_wf.push_back({"fetched_instruction", fetched_instruction});
            std::cout << fetched_instruction << std::endl;
            prevprevPC_wf = prevPC_wf;
            prevPC_wf = PC;
            PC += 1;
            ins_wf++;
            std::cout<<fetched_instruction<<std::endl;
        }
        else
        {
            eof_wf = true;
            std::cout << "Reached eof" << std::endl;
        }
        return;
    }
    void DecodeWF()
    {
        if (!stall_flag_wf && !stall_flag2_wf)
        {
            std::cout << "in decode" << std::endl;
            latch_IDRF_wf = latch_IF_wf;
        }
        std::cout << "f ins"<<search_latch("fetched_instruction", latch_IDRF_wf)<<std::endl;
        std::istringstream iss(search_latch("fetched_instruction", latch_IDRF_wf));
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
        std::string prev_instruction = instructions[prevprevPC_wf].first;
        if (prev_instruction.back()==':')prevprevPC_wf--;
        prev_instruction = instructions[prevprevPC_wf].first;
        std::cout<< prev_instruction<<std::endl;
        std::istringstream iss_prev(prev_instruction);
        std::vector<std::string> pt;
      
        while (iss_prev >> token)
        {
            pt.push_back(token);
        }
       
        std::string prev_opcode;
        std::string prev_rd;
        
        if (!pt.empty() )
        {
          
            prev_opcode = pt[0];
            if(pt.size() >1){
            prev_rd = pt[1];
            }
            std::cout << "PREV OPCODE" << prev_opcode << std::endl;
        }

        if (std::find(ins_type1_wf.begin(), ins_type1_wf.end(), opcode) != ins_type1_wf.end())
        {

            std::string rs1 = tokens[2];
            std::string rs2 = tokens[3];
            int rs1_value;
            int rs2_value;
            rs1num = returnIndex(rs1);
            rs2num = returnIndex(rs2);
         
            if (prev_opcode != "lw" && !ishazard_notified_wf)
            {
                std::cout << "prev is not  lw" << std::endl;
                if (isHazard(rs1, latch_EXE_wf))
                {
                    rs1_value = stoi(search_latch("result", latch_EXE_wf));
                }
                if (isHazard(rs2, latch_EXE_wf))
                {
                    rs2_value = stoi(search_latch("result", latch_EXE_wf));
                }

                else
                {
                    rs1_value = reg[rs1num];
                    rs2_value = reg[rs2num];
                }
            }
            else
            {
                // std::string prev_mem_op=search_latch()
                if (isHazard(rs1, latch_EXE_wf) && !ishazard_notified_wf)
                {
                    stall_flag_wf = true;
                    stalls_wf += 1;
                    ishazard_notified_wf = true;
                }
                if (isHazard(rs1, latch_EXE_wf) && !ishazard_notified_wf)
                {
                    stall_flag2_wf = true;
                    stalls_wf += 1;
                    ishazard_notified_wf = true;
                }

                else
                {
                    stall_flag_wf = false;
                    stall_flag2_wf = false;
                    ishazard_notified_wf = false;
                    rs1_value = reg[rs1num];
                    rs2_value = reg[rs2num];
                }
            }
            if (!stall_flag_wf && !stall_flag2_wf)
            {
                latch_IDRF_wf.push_back({"Opcode", opcode});
                latch_IDRF_wf.push_back({"rd", rd});
                latch_IDRF_wf.push_back({"rs1", rs1});
                latch_IDRF_wf.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF_wf.push_back({"rs2", rs2});
                latch_IDRF_wf.push_back({"rs2_value", std::to_string(rs2_value)});
            }

            // reg_state[returnIndex(rd)] = 'd';
        }
        if (std::find(ins_type2_wf.begin(), ins_type2_wf.end(), opcode) != ins_type2_wf.end())
        {
            int rs1_value;
            std::string rs1 = tokens[2];
            int rs1num = returnIndex(rs1);
            if (prev_opcode != "lw" && !ishazard_notified_wf)
            {
                if (isHazard(rs1, latch_EXE_wf))
                {
                    rs1_value = stoi(search_latch("result", latch_EXE_wf));
                }

                else
                {
                    rs1_value = reg[rs1num];
                }
            }
            else
            {
                // std::string prev_mem_op=search_latch()
                if (isHazard(rs1, latch_EXE_wf) && !ishazard_notified_wf)
                {
                    stall_flag_wf = true;
                    stalls_wf += 1;
                    ishazard_notified_wf = true;
                }

                else
                {
                    stall_flag_wf = false;
                    stall_flag2_wf = false;
                    ishazard_notified_wf = false;
                    rs1_value = reg[rs1num];
                }
            }
            if (!stall_flag_wf && !stall_flag2_wf)
            {
                latch_IDRF_wf.push_back({"Opcode", opcode});
                latch_IDRF_wf.push_back({"rd", rd});
                latch_IDRF_wf.push_back({"rs1", rs1});
                latch_IDRF_wf.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF_wf.push_back({"immediate", tokens[3]});
                reg_state[returnIndex(rd)] = 2;
            }
        }
        if (std::find(ins_type3_wf.begin(), ins_type3_wf.end(), opcode) != ins_type3_wf.end())
        {
          
            branch_flag_wf = true;
            executed_branch_wf = false;
            stalls_wf += 1;
            // count-=2;
            std::string rs2 = tokens[2];

            std::string rs1 = tokens[1];
            int rs1_value;
            int rs2_value;
            rs1num = returnIndex(rs1);
            rs2num = returnIndex(rs2);
            std::cout << "WHILE DEC BRANCH REG STATE" << reg_state[rs1num] << std::endl;
            if (prev_opcode != "lw" && !ishazard_notified_wf)
            {
                if (isHazard(rs1, latch_EXE_wf))
                {
                    rs1_value = stoi(search_latch("result", latch_EXE_wf));
                }
                if (isHazard(rs2, latch_EXE_wf))
                {
                    rs2_value = stoi(search_latch("result", latch_EXE_wf));
                }

                else
                {
                    rs1_value = reg[rs1num];
                    rs2_value = reg[rs2num];
                }
            }
            else
            {
                // std::string prev_mem_op=search_latch()
                if (isHazard(rs1, latch_EXE_wf) && !ishazard_notified_wf)
                {
                    stall_flag_wf = true;
                    stalls_wf += 1;
                    ishazard_notified_wf = true;
                }
                if (isHazard(rs1, latch_EXE_wf) && !ishazard_notified_wf)
                {
                    stall_flag2_wf = true;
                    stalls_wf += 1;
                    ishazard_notified_wf = true;
                }

                else
                {
                    stall_flag_wf = false;
                    stall_flag2_wf = false;
                    ishazard_notified_wf = false;
                    rs1_value = reg[rs1num];
                    rs2_value = reg[rs2num];
                }
            }

            if (!stall_flag_wf && !stall_flag2_wf)
            {
                int result = control_executions(opcode, rs1_value, rs2_value);
                // std::cout << "EXECUTED BRANCH" << result << std::endl;
                // std::cout << result << std::endl;
                std::string label = tokens[3];
                if (result != predict_branch())
                {
                    mis_predict_wf == true;
                    stalls_wf += 1;
                    // PC = findLabelIndex(label);
                    // std::cout<<"label ind"<<findLabelIndex(label);
                }

                branch_flag_wf = false;
                latch_IDRF_wf.push_back({"Opcode", opcode});
                latch_IDRF_wf.push_back({"rs1", rd});
                latch_IDRF_wf.push_back({"rs2", rs2});
                latch_IDRF_wf.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF_wf.push_back({"rs2_value", std::to_string(rs2_value)});
                latch_IDRF_wf.push_back({"Label", tokens[3]});
            }
        }
        if (std::find(ins_type4_wf.begin(), ins_type4_wf.end(), opcode) != ins_type4_wf.end())
        {
          //  std::cout << "decoding type4" << std::endl;
            std::string offset;
            int baseRegister_value;
            int rs1num;
            std::string offsetBase = tokens[2];
            size_t openParen = offsetBase.find_first_of('(');
            std::string baseRegister;
            int rs1_value;
            baseRegister = offsetBase.substr(openParen + 1, offsetBase.size() - openParen - 2);

            rs1num = returnIndex(baseRegister);

            offset = offsetBase.substr(0, openParen);
            int rdnum = returnIndex(rd);
            int rd_value;
            if (opcode == "lw")
            {
                if (prev_opcode != "lw")
                {
                    if (isHazard(baseRegister, latch_MEM_wf))
                    {
                        rs1_value = stoi(search_latch("result", latch_MEM_wf));
                    }

                    else
                    {
                        rs1_value = reg[rs1num];
                    }
                }
                else
                {
                    if (isHazard(rd, latch_MEM_wf))
                    {
                        stall_flag_wf = true;
                        stalls_wf += 1;
                    }

                    else
                    {
                        stall_flag_wf = false;
                        // stall_flag2 = false;
                        rs1_value = reg[rs1num];
                    }
                }
                if (!stall_flag_wf && !stall_flag2_wf)
                {
                    latch_IDRF_wf.push_back({"Opcode", opcode});
                    latch_IDRF_wf.push_back({"rd", rd});
                    latch_IDRF_wf.push_back({"baseReg_value", std::to_string(rs1_value)});
                    latch_IDRF_wf.push_back({"baseRegister", baseRegister});
                    latch_IDRF_wf.push_back({"Offset", offset});
                    reg_state[rdnum] = 2;
                }
            }
            else if (opcode == "sw")
            {
               // std::cout << "DECODED sw" << std::endl;
                std::cout << reg_state[rdnum] << rdnum << std::endl;
                std::cout << reg_state[rs1num] << rs1num << std::endl;
                if (prev_opcode != "lw")
                {
                    if (isHazard(rd, latch_MEM_wf))
                    {
                        rd_value = stoi(search_latch("result", latch_MEM_wf));
                    }
                    else if (isHazard(baseRegister, latch_MEM_wf))
                    {
                        baseRegister_value = stoi(search_latch("result", latch_MEM_wf));
                    }
                    else
                    {
                        baseRegister_value = reg[rs1num];
                        rd_value = reg[rdnum];
                    }
                }
                else
                {
                    if (isHazard(rd, latch_MEM_wf))
                    {
                        stall_flag_wf = true;
                        stalls_wf += 1;
                        // rd_value=stoi(search_latch("result",latch_MEM));
                    }
                    else if (isHazard(baseRegister, latch_MEM_wf))
                    {
                        stall_flag2_wf = true;
                        stalls_wf += 1;
                        // baseRegister_value=stoi(search_latch("result",latch_MEM));
                    }
                    else
                    {
                        baseRegister_value = reg[rs1num];
                        rd_value = reg[rdnum];
                        stall_flag_wf = false;
                        stall_flag2_wf = false;
                    }
                }
                if (!stall_flag_wf && !stall_flag2_wf)
                {
                    latch_IDRF_wf.push_back({"Opcode", opcode});
                    latch_IDRF_wf.push_back({"baseRegister", baseRegister});
                    latch_IDRF_wf.push_back({"baseReg_value", std::to_string(baseRegister_value)});
                    latch_IDRF_wf.push_back({"Offset", offset});
                    latch_IDRF_wf.push_back({"rd", rd});
                    latch_IDRF_wf.push_back({"rd_val", std::to_string(rd_value)});
                    reg_state[rdnum] = 2;
                }
            }
            else if (opcode == "la")
            {
                latch_IDRF_wf.push_back({"Opcode", opcode});
                latch_IDRF_wf.push_back({"rd", rd});
                latch_IDRF_wf.push_back({"Label", tokens[2]});
            }
        }
        if (std::find(ins_type5_wf.begin(), ins_type5_wf.end(), opcode) != ins_type5_wf.end())
        {
            branch_flag_wf = true;

            executed_branch_wf = false;
            latch_IDRF_wf.push_back({"Opcode", opcode});
            if (opcode == "j" || opcode == "jal")
            {
               // PC = findLabelIndex(rd);

                latch_IDRF_wf.push_back({"Label", rd});
            }
            if (opcode == "jalr")
            {
               
                int rs1Num = returnIndex(tokens[2]);
                int rs1_value = reg[rs1Num];

                latch_IDRF_wf.push_back({"rd", rd});
                latch_IDRF_wf.push_back({"rs1", tokens[2]});
                latch_IDRF_wf.push_back({"rs1_value", std::to_string(rs1_value)});
                latch_IDRF_wf.push_back({"Offset", tokens[2]});
                reg_state[returnIndex(rd)] = 2;
            }
        }

        if (stall_flag_wf || stall_flag2_wf)
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
        return res;
    }

    void ExecuteWF(std::vector<std::pair<std::string, std::string>> latch_IDRF_wf)
    {
        latch_EXE_wf = latch_IDRF_wf;
        std::string opcode;
        std::string rd;
        int result = 0;
        int updatedPC = PC;
        for (const auto &pair : latch_IDRF_wf)
        {
            if (pair.first == "Opcode")
            {
                opcode = pair.second;
            }
        }
        int op1 = 0, op2 = 0;
        for (const auto &pair : latch_IDRF_wf)
        {
            if (pair.first == "rs1_value")
            {
                //  std::cout<<"not this"<<std::endl;
                op1 = stoi(pair.second);
            }
        }
        for (const auto &pair : latch_IDRF_wf)
        {
            if (pair.first == "immediate")
            {

                op2 = stoi(pair.second);
                // std::cout<<"not this"<<std::endl;
            }
            else if (pair.first == "rs2_value")
            {

                op2 = stoi(pair.second);
                // std::cout<<"not this"<<std::endl;
            }
        }

        for (const auto &pair : latch_IDRF_wf)
        {
            if (pair.first == "rd")
            {
                rd = pair.second;
            }
        }
        if (std::find(ins_type1_wf.begin(), ins_type1_wf.end(), opcode) != ins_type1_wf.end() || std::find(ins_type2_wf.begin(), ins_type2_wf.end(), opcode) != ins_type2_wf.end())
        {

            if (opcode == "add" || opcode == "sub" || opcode == "addi")
            {

                result = executeArithmeticInstruction(op1, op2, opcode);
            }
            reg[returnIndex(rd)] = result;
            reg_state[returnIndex(rd)] = 3;
        }
        if (std::find(ins_type3_wf.begin(), ins_type3_wf.end(), opcode) != ins_type3_wf.end())
        {
            for (const auto &pair : latch_EXE_wf)
            {
                std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
            }
            std::cout << op1 << " " << op2 << std::endl;
            result = control_executions(opcode, op1, op2);
            // std::cout << "EXECUTED BRANCH" << result << std::endl;
            // std::cout << result << std::endl;
            std::string label = search_latch("Label", latch_IDRF_wf);
            if (result == 1)
            {
                PC = findLabelIndex(label);
               // std::cout << "label ind" << findLabelIndex(label);
            }
            branch_flag_wf = false;
            executed_branch_wf = true;
        }
        if (std::find(ins_type4_wf.begin(), ins_type4_wf.end(), opcode) != ins_type4_wf.end())
        {
            std::cout << "in" << std::endl;
            int baseReg = 0;
            int offset;

            for (const auto &pair : latch_IDRF_wf)
            {
                if (pair.first == "baseReg_value")
                {
                   // std::cout << "First: " << std::endl;
                    baseReg = stoi(pair.second);
                }
            }
            for (const auto &pair : latch_IDRF_wf)
            {
                if (pair.first == "Offset")
                {
                    offset = stoi(pair.second);
                }
            }
            result = baseReg + offset;
            for (const auto &pair : latch_IDRF_wf)
            {
                if (pair.first == "rd_value")
                {
                    op2 = stoi(pair.second);
                }
            }
            if (opcode != "la")
            {
                latch_EXE_wf.push_back({"rs1_value", std::to_string(op2)});
            }
            if (opcode == "la")
            {
               // std::cout << "ff la" << std::endl;
                std::string lbl;
                for (const auto &pair : latch_EXE_wf)
                {
                    if (pair.first == "Label")
                    {
                        std::cout << "found the label" << std::endl;
                        lbl = pair.second;
                    }
                }
                if (labelToAddress.find(lbl) != labelToAddress.end())
                {
                    //loaded_value=labelToAddress[lbl];
                    reg[returnIndex(rd)] = labelToAddress[lbl];
                }
            }

            reg_state[returnIndex(rd)] = 3;
        }
        if (std::find(ins_type5_wf.begin(), ins_type5_wf.end(), opcode) != ins_type5_wf.end())

        {
            if(opcode == "j"){
                  std::string label = search_latch("Label", latch_IDRF_wf);
                PC = findLabelIndex(label);
            }
            if (opcode == "jalr")
            {
                reg[returnIndex(rd)] = PC + 1;
                // look into this
            }
        }
        if (opcode != "la" || opcode != "j")
        {
            latch_EXE_wf.push_back({"result", std::to_string(result)});
        }
    }
    void MemoryWF(std::vector<std::pair<std::string, std::string>> latch_EXE_wf, char *RAM, Cache_simulator *sim_cache)
    {
        //std::cout << "MEM opcode" << std::endl;
        std::string rd;
        rd = search_latch("rd", latch_EXE_wf);
        std::string opcode;
        latch_MEM_wf = latch_EXE_wf;
        for (const auto &pair : latch_EXE_wf)
        {
            if (pair.first == "Opcode")
            {
                std::cout << "MEM opcode" << std::endl;
                opcode = pair.second;
            }
        }
        int result;

        for (const auto &pair : latch_EXE_wf)
        {
            if (pair.first == "result")
            {

                result = stoi(pair.second);
                std::cout << result << std::endl;
            }
        }

        if (std::find(ins_type1_wf.begin(), ins_type1_wf.end(), opcode) != ins_type1_wf.end() || std::find(ins_type2_wf.begin(), ins_type2_wf.end(), opcode) != ins_type2_wf.end())
        {
            latch_MEM_wf = latch_EXE_wf;
            reg_state[returnIndex(rd)] = 4;
        }
        if (std::find(ins_type3_wf.begin(), ins_type3_wf.end(), opcode) != ins_type3_wf.end())
        {
            //  executed_branch = true;
        }
        if (std::find(ins_type4_wf.begin(), ins_type4_wf.end(), opcode) != ins_type4_wf.end())

        {
            int loaded_value = 0;

            if (opcode == "lw")
            {
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
                       // std::cout << "ff lw" << std::endl;
                    }
                }
               // std::cout << "succesful" << std::endl;
                reg_state[returnIndex(rd)] = 4;
                //  reg[returnIndex(rd)] = loaded_value;
            }
            else if (opcode == "sw")
            {
                std::cout << "ff sw" << std::endl;
                int load_value;
                for (const auto &pair : latch_EXE_wf)
                {
                    if (pair.first == "rd_value")
                    {
                        //std::cout << "I m the problem" << std::endl;
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
               // std::cout << "ff la" << std::endl;
                std::string lbl;
                for (const auto &pair : latch_EXE_wf)
                {
                    if (pair.first == "Label")
                    {
                        //std::cout << "found the label" << std::endl;
                        lbl = pair.second;
                    }
                }
                if (labelToAddress.find(lbl) != labelToAddress.end())
                {
                    loaded_value = labelToAddress[lbl];
                    reg[returnIndex(rd)] = labelToAddress[lbl];
                    hit = sim_cache->access(loaded_value);
                    if (hit)
                        hits++;
                    if (!hit)
                        miss = true;
                }
            }

            latch_MEM_wf.push_back({"loaded_value", std::to_string(loaded_value)});
        }
        if (std::find(ins_type5_wf.begin(), ins_type5_wf.end(), opcode) != ins_type5_wf.end())
        {
            executed_branch_wf = true;
            j = true;
        }
    }

    void WriteBackWF(std::vector<std::pair<std::string, std::string>> latch_MEM_wf)
    {

        std::string opcode = search_latch("Opcode", latch_MEM_wf);

        std::string rd = search_latch("rd", latch_MEM_wf);
        // std::cout<<opcode<<std::endl;
        if (std::find(ins_type1_wf.begin(), ins_type1_wf.end(), opcode) != ins_type1_wf.end() || std::find(ins_type2_wf.begin(), ins_type2_wf.end(), opcode) != ins_type2_wf.end())
        {
            int result = stoi(search_latch("result", latch_MEM_wf));

            std::cout << "e" << std::endl;
            reg[returnIndex(rd)] = result;
            reg_state[returnIndex(rd)] = 5;
        }
        if (std::find(ins_type3_wf.begin(), ins_type3_wf.end(), opcode) != ins_type3_wf.end())
        {
        }
        if (std::find(ins_type4_wf.begin(), ins_type4_wf.end(), opcode) != ins_type4_wf.end())
        {
            if (opcode == "lw")
            {
                std::string loaded_value = search_latch("loaded_value", latch_MEM_wf);
                reg[returnIndex(rd)] = stoi(loaded_value);
                // reg_state[returnIndex(rd)] = 5;
            }
            if (opcode == "sw")
            {
            }
            if (opcode == "la")
            {
                std::string loaded_value = search_latch("loaded_value", latch_MEM_wf);
                //  std::cout << "Val of ind" << loaded_value << std::endl;
                reg[returnIndex(rd)] = stoi(loaded_value);
                reg_state[returnIndex(rd)] = 5;

                std::cout << rd << std::endl;
            }
            reg_state[returnIndex(rd)] = 5;
        }
        if (std::find(ins_type4_wf.begin(), ins_type4_wf.end(), opcode) != ins_type4_wf.end())
        {
            if (opcode == "jalr")
            {
                reg[returnIndex(rd)] = PC + 1;
                // look into this
            }
        }
    }

    void Step_countWF(char *RAM, Cache_simulator *sim_cache)
    {
        int latency_addi = latency_map["ADDI"] - 1;
        int latency_add = latency_map["ADD"] - 1;
        int latency_mul = latency_map["MUL"] - 1;
        int latency_sub = latency_map["SUB"] - 1;
        int mem_access_latency = sim_cache->get_mem_latency() - 1;
        int cache_latency = sim_cache->get_cache_latency() - 1;
        int mem_access_latency_f = sim_cache->get_mem_latency() - 1;
        int cache_latency_f = sim_cache->get_cache_latency() - 1;
        int c = 1;
        int y = 0, z = 0;
        int k = 0;
        bool keep_going_wf = true;
        while (keep_going_wf)
        {
            y = 0;
            c++;
            bool cont = false;
            if (latch_MEM_wf.size() > 0)
            {
                k = 1;
                std::cout << "WB";
                for (const auto &pair : latch_MEM_wf)
                {
                    std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
                }
              //  pip[y + z][c] = "W";
                z++;
                WriteBackWF(latch_MEM_wf);
                latch_MEM_wf.clear();
               
            }
            if (latch_EXE_wf.size() > 0 && (!hit) && !miss)
            {

                k = 2;
               // pip[y + z][c] = "M";
                y++;
                if (!hit && !miss)
                {
                    MemoryWF(latch_EXE_wf, RAM, sim_cache);
                }
                std::string opcode = search_latch("Opcode", latch_EXE_wf);
                if (opcode == "lw" || opcode == "la" || opcode == "sw")
                {
                    if (miss)
                    {
                        if (mem_access_latency > 0)
                        {
                           // std::cout << " i ran " << opcode << mem_access_latency << std::endl;
                            mem_access_latency--;
                            loop_wf++;
                            cont = true;
                        }
                    }
                    else if (hit)
                    {
                        if (cache_latency > 0)
                        {
                            cache_latency--;
                            loop_wf++;
                            cont = true;
                        }
                    }
                }
                if (cont == true)
                {
                    continue;
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

                latch_EXE_wf.clear();
                std::cout << "Memo";
            }
            if (latch_IDRF_wf.size() > 0 )
            {
                bool f = false;
                if (!stall_flag_wf && !stall_flag2_wf)
                {
                    k = 3;
                   
                    std::string op = search_latch("Opcode", latch_IDRF_wf);

                   // pip[y + z][c] = "E";
                    y++;
                    // std::cout << "the lat" << latency_addi << std::endl;
                    if (op == "addi")
                    {
                        if (latency_addi > 0)
                        {
                            f = true;
                            // std::cout << "latin" << std::endl;
                            latency_addi--;
                             stalls_wf++;
                            loop_wf++;
                            continue;
                        }
                    }
                    else if (op == "add")
                    {
                        if (latency_add > 0)
                        {
                            f = true;
                             stalls_wf++;
                            latency_add--;
                            loop_wf++;
                            continue;
                        }
                    }
                    else if (op == "sub")
                    {
                        if (latency_sub > 0)
                        {
                            f = true;
                             stalls_wf++;
                            latency_sub--;
                            loop_wf++;
                            continue;
                        }
                    }
                    else if (op == "mul")
                    {
                        if (latency_mul > 0)
                        {
                            f = true;
                            stalls_wf++;
                            latency_mul--;
                            loop_wf++;
                            continue;
                        }
                    }
                    if (f)
                    {
                        continue;
                    }
                    ExecuteWF(latch_IDRF_wf);
                    if (executed_branch_wf && mis_predict_wf)
                    {
                        PC = (findLabelIndex(search_latch("Label", latch_EXE_wf))) + 1;
                        latch_IF_wf.clear();
                        executed_branch_wf = false;
                        mis_predict_wf = false;
                    }
                    latency_addi = latency_map["ADDI"] - 1;
                    latency_add = latency_map["ADD"] - 1;
                    latency_mul = latency_map["MUL"] - 1;
                    latency_sub = latency_map["SUB"] - 1;
                    latch_IDRF_wf.clear();
                     std::cout << "E";
                }
            }
            if (!miss_fetch && !hit_fetch)
            {
               std::cout<<"en"<<std::endl;
               std::cout<<ishazard_notified_wf<<std::endl;
                if (latch_IF_wf.size() > 0 || ishazard_notified_wf)
                {
                     std::cout << "en";
                    if (eof_wf && !deof_wf)
                    {
                        if (latch_IDRF_wf.size() == 0)
                        {
                            k = 4;
                          //  pip[y + z][c] = "D";
                            y++;
                            DecodeWF();

                           // std::cout << "ID";
                            latch_IF_wf.clear();
                            deof_wf = true;
                        }
                        else if ((stall_flag_wf || stall_flag2_wf) && latch_IDRF_wf.size() == 2)
                        {
                            k = 4;
                           // pip[y + z][c] = "D";
                            y++;
                            DecodeWF();
                            deof_wf = true;
                        }
                    }
                    else
                    {
                        std::cout<<latch_IDRF_wf.size()<<std::endl;
                        if (latch_IDRF_wf.size() == 0)
                        {
                            k = 4;
                           // pip[y + z][c] = "D";
                            y++;
                            DecodeWF();

                            std::cout << "ID";
                            latch_IF_wf.clear();
                        }
                        else if ((stall_flag_wf || stall_flag2_wf) && latch_IDRF_wf.size() == 2)
                        {
                            k = 4;
                           // pip[y + z][c] = "D";
                            y++;
                            DecodeWF();

                            std::cout << "ID";
                        }
                    }
                }
                else if(eof_wf && !ishazard_notified_wf){
                      k = 4;
                           // pip[y + z][c] = "D";
                            y++;
                            DecodeWF();

                            std::cout << "ID";
                }
            }
             //  std::cout<<"ex"<<executed_branch_wf<<mis_predict_wf<<std::endl;
            if (latch_IF_wf.size() == 0 && !eof_wf ||(latch_IF_wf.size()==2 &&(miss_fetch || hit_fetch)) )
            {
                k = 5;

            //    pip[y + z][c] = "F";
              //  pip[y + z][0] = std::to_string(PC);
                y++;
                if (!miss_fetch && !hit_fetch)
                {
                    FetchWF(sim_cache);
                }
                 if (miss_fetch)
                {
                    if (mem_access_latency_f > 0)
                    {
                       //  std::cout << " i ran "  << std::endl;
                        mem_access_latency_f--;
                        stalls_wf++;
                        loop_wf++;
                        cont = true;
                    }
                }
                else if (hit_fetch)
                {
                    if (cache_latency_f > 0)
                    {
                        cache_latency_f--;
                        stalls_wf++;
                        loop_wf++;
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
           
            else if(eof_wf && executed_branch_wf && !mis_predict_wf)
            {
                //std::cout << "came here but a stall;)" << std::endl;
                break;
             
            }
            // else if(eof_wf && j){
            //     break;
            // }
            std::cout << k << std::endl;
            // if (v == 0)
            //     break;
            if (k == 0)
            {
                keep_going_wf = false;
                break;
            }
            else
            {
                loop_wf++;
                std::cout << loop_wf << std::endl;
            }
            std::cout << "\nONE cycle finish\n";
            v--;
           // if(v==0 )break;
        }
        //std::cout << instructions.size() << std::endl;
        // std::cout << stalls_wf << std::endl;
        // std::cout << loop_wf << std::endl;
        std::cout << "WIth Forwarding" << std::endl;
        std::cout << "No of stalls" << std::endl;
        std::cout << stalls_wf << std::endl;
        std::cout << "No of cycles" << std::endl;
        std::cout << loop_wf << std::endl;
        std::cout << "Instructions per cycle" << std::endl;
        std::cout << loop_wf / (float)ins_wf << std::endl;
        std::cout << "Hit Rate" << std::endl;
        std::cout << hits / accesses << std::endl;
        // for (int i = 0; i < 30; i++)
        // {
        //     for (int j = 0; j < 30; j++)
        //     {
        //         std::cout << pip[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        for (int i = 0; i < 32; i++)
        {
        }
    }

    // int main()
    // {
    //     char RAM[4096];
    //     int clock = 0;
    //     bool visited[4096] = {0};
    //     int x;
    //     std::cout << "Enter the latency for ADD:\n" << std::endl;
    //     std::cin >> x;
    //     latency_map["ADD"] = x;
    //     std::cout << "Enter the latency for SUB:\n" << std::endl;
    //     std::cin >> x;
    //     latency_map["SUB"] = x;
    //     std::cout << "Enter the latency for ADDI:\n" << std::endl;
    //     std::cin >> x;
    //     latency_map["ADDI"]=x;
    //     std::ifstream instructionsFile("text.txt");
    //     if (!instructionsFile.is_open())
    //     {
    //         std::cerr << "Error opening file." << std::endl;
    //     }
    //     Core core1(instructionsFile);
    //     core1.readInstructionsFromFile("text.txt", RAM, visited);
    //     core1.Step_countWF(RAM);
    //     //    for(int i=0;i<4096;i++){
    //     //     std::cout<<RAM[i]<<std::endl;
    //     //    }
    //     instructionsFile.close();
    //     return 0;
    // }
};
#endif
