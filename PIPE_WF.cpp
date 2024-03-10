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

void Core::FetchWF()
{
    if (PC < instructions.size())
    {
        std::string fetched_instruction = instructions[PC].first;
        if (fetched_instruction.back() == ':')
        {
            PC = PC + 1;
            fetched_instruction = instructions[PC].first;
        }
        latch_IF.push_back({"PC", std::to_string(PC)});
        latch_IF.push_back({"fetched_instruction", fetched_instruction});
        std::cout << fetched_instruction << std::endl;
        prevprevPC = prevPC;
        prevPC = PC;
        PC += 1;
        ins++;
        std::cout << " bloody i" << ins << std::endl;
    }
    else
    {
        eof = true;
        std::cout << "Reached eof" << std::endl;
    }
    return;
}
void Core::DecodeWF()
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
    std::string prev_instruction = instructions[prevprevPC].first;
    std::istringstream iss_prev(prev_instruction);
    std::vector<std::string> pt;
    while (iss_prev >> token)
    {
        pt.push_back(token);
    }
    std::string prev_opcode;
    std::string prev_rd;
    if (!tokens.empty())
    {
        prev_opcode = pt[0];
        prev_rd = pt[1];
        std::cout << "PREV OPCODE" << prev_opcode << std::endl;
    }

    if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end())
    {

        std::string rs1 = tokens[2];
        std::string rs2 = tokens[3];
        int rs1_value;
        int rs2_value;
        rs1num = returnIndex(rs1);
        rs2num = returnIndex(rs2);
        // if (prev_opcode != "lw")
        // {
        //     if (isHazard(rs1, latch_MEM))
        //     {
        //         rs1_value = stoi(search_latch("result", latch_MEM));
        //     }
        //     else if (isHazard(rs2, latch_MEM))
        //     {
        //         rs2_value = stoi(search_latch("result", latch_MEM));
        //     }
        //     else
        //     {
        //         rs1_value = reg[rs1num];
        //         rs2_value = reg[rs2num];
        //     }
        // }
        // else
        // {
        //     if (isHazard(rs1, latch_MEM))
        //     {
        //         stall_flag = true;
        //         stalls += 1;
        //     }
        //     else if (isHazard(rs2, latch_MEM))
        //     {
        //         stall_flag2 = true;
        //         stalls += 1;
        //     }
        //     else
        //     {
        //         stall_flag = false;
        //         stall_flag2 = false;
        //         rs1_value = reg[rs1num];
        //         rs2_value = reg[rs2num];
        //     }
        // }
        if (prev_opcode != "lw" && !ishazard_notified)
        {
            std::cout << "prev is not  lw" << std::endl;
            if (isHazard(rs1, latch_EXE))
            {
                rs1_value = stoi(search_latch("result", latch_EXE));
            }
            if (isHazard(rs2, latch_EXE))
            {
                rs2_value = stoi(search_latch("result", latch_EXE));
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
            if (isHazard(rs1, latch_EXE) && !ishazard_notified)
            {
                stall_flag = true;
                stalls += 1;
                ishazard_notified = true;
            }
            if (isHazard(rs1, latch_EXE) && !ishazard_notified)
            {
                stall_flag2 = true;
                stalls += 1;
                ishazard_notified = true;
            }

            else
            {
                stall_flag = false;
                stall_flag2 = false;
                ishazard_notified = false;
                rs1_value = reg[rs1num];
                rs2_value = reg[rs2num];
            }
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

        // reg_state[returnIndex(rd)] = 'd';
    }
    if (std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
    {
        int rs1_value;
        std::string rs1 = tokens[2];
        int rs1num = returnIndex(rs1);
        if (prev_opcode != "lw" && !ishazard_notified)
        {
            std::cout << "prev is lw" << std::endl;
            if (isHazard(rs1, latch_EXE))
            {
                rs1_value = stoi(search_latch("result", latch_EXE));
            }

            else
            {
                rs1_value = reg[rs1num];
            }
        }
        else
        {
            // std::string prev_mem_op=search_latch()
            if (isHazard(rs1, latch_EXE) && !ishazard_notified)
            {
                stall_flag = true;
                stalls += 1;
                ishazard_notified = true;
            }

            else
            {
                stall_flag = false;
                stall_flag2 = false;
                ishazard_notified = false;
                rs1_value = reg[rs1num];
            }
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
        stalls += 1;
        // count-=2;
        std::string rs2 = tokens[2];

        std::string rs1 = tokens[1];
        int rs1_value;
        int rs2_value;
        rs1num = returnIndex(rs1);
        rs2num = returnIndex(rs2);
        std::cout << "WHILE DEC BRANCH REG STATE" << reg_state[rs1num] << std::endl;
        if (prev_opcode != "lw" && !ishazard_notified)
        {
            std::cout << "prev is not  lw" << std::endl;
            if (isHazard(rs1, latch_EXE))
            {
                rs1_value = stoi(search_latch("result", latch_EXE));
            }
            if (isHazard(rs2, latch_EXE))
            {
                rs2_value = stoi(search_latch("result", latch_EXE));
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
            if (isHazard(rs1, latch_EXE) && !ishazard_notified)
            {
                stall_flag = true;
                stalls += 1;
                ishazard_notified = true;
            }
            if (isHazard(rs1, latch_EXE) && !ishazard_notified)
            {
                stall_flag2 = true;
                stalls += 1;
                ishazard_notified = true;
            }

            else
            {
                stall_flag = false;
                stall_flag2 = false;
                ishazard_notified = false;
                rs1_value = reg[rs1num];
                rs2_value = reg[rs2num];
            }
        }

        if (!stall_flag && !stall_flag2)
        {
            int result = control_executions(opcode, rs1_value, rs2_value);
            std::cout << "EXECUTED BRANCH" << result << std::endl;
            std::cout << result << std::endl;
            std::string label = tokens[3];
            if (result != predict_branch())
            {
                mis_predict == true;
                stalls += 1;
                // PC = findLabelIndex(label);
                // std::cout<<"label ind"<<findLabelIndex(label);
            }

            branch_flag = false;
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
        std::cout << "decoding type4" << std::endl;
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
                if (isHazard(baseRegister, latch_MEM))
                {
                    rs1_value = stoi(search_latch("result", latch_MEM));
                }

                else
                {
                    rs1_value = reg[rs1num];
                }
            }
            else
            {
                if (isHazard(rd, latch_MEM))
                {
                    stall_flag = true;
                    stalls += 1;
                }

                else
                {
                    stall_flag = false;
                    // stall_flag2 = false;
                    rs1_value = reg[rs1num];
                }
            }
            if (!stall_flag && !stall_flag2)
            {
                latch_IDRF.push_back({"Opcode", opcode});
                latch_IDRF.push_back({"rd", rd});
                latch_IDRF.push_back({"baseReg_value", std::to_string(rs1_value)});
                latch_IDRF.push_back({"baseRegister", baseRegister});
                latch_IDRF.push_back({"Offset", offset});
                reg_state[rdnum] = 2;
            }
           
        }
        else if (opcode == "sw")
        {
            std::cout << "DECODED sw" << std::endl;
            std::cout << reg_state[rdnum] << rdnum << std::endl;
            std::cout << reg_state[rs1num] << rs1num << std::endl;
            if (prev_opcode != "lw")
            {
                if (isHazard(rd, latch_MEM))
                {
                    rd_value = stoi(search_latch("result", latch_MEM));
                }
                else if (isHazard(baseRegister, latch_MEM))
                {
                    baseRegister_value = stoi(search_latch("result", latch_MEM));
                }
                else
                {
                    baseRegister_value = reg[rs1num];
                    rd_value = reg[rdnum];
                }
            }
            else
            {
                if (isHazard(rd, latch_MEM))
                {
                    stall_flag = true;
                    stalls += 1;
                    // rd_value=stoi(search_latch("result",latch_MEM));
                }
                else if (isHazard(baseRegister, latch_MEM))
                {
                    stall_flag2 = true;
                    stalls += 1;
                    // baseRegister_value=stoi(search_latch("result",latch_MEM));
                }
                else
                {
                    baseRegister_value = reg[rs1num];
                    rd_value = reg[rdnum];
                    stall_flag = false;
                    stall_flag2 = false;
                }
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

        executed_branch = false;
        latch_IDRF.push_back({"Opcode", opcode});
        if (opcode == "j" || opcode == "jal")
        {
            PC = findLabelIndex(rd);

            latch_IDRF.push_back({"Label", rd});
        }
        if (opcode == "jalr")
        {
            // look into this...if time permits
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
    return res;
}

void Core::ExecuteWF(std::vector<std::pair<std::string, std::string>> latch_IDRF)
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
            //  std::cout<<"not this"<<std::endl;
            op1 = stoi(pair.second);
        }
    }
    for (const auto &pair : latch_IDRF)
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

    for (const auto &pair : latch_IDRF)
    {
        if (pair.first == "rd")
        {
            rd = pair.second;
        }
    }
    if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end() || std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
    {

        if (opcode == "add" || opcode == "sub" || opcode == "addi")
        {

            result = executeArithmeticInstruction(op1, op2, opcode);
        }
        reg[returnIndex(rd)] = result;
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
        std::cout << result << std::endl;
        std::string label = search_latch("Label", latch_IDRF);
        if (result == 1)
        {
            PC = findLabelIndex(label);
            std::cout << "label ind" << findLabelIndex(label);
        }
        branch_flag = false;
        executed_branch = true;
    }
    if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())
    {
        std::cout << "in" << std::endl;
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
        if (opcode == "la")
        {
            std::cout << "ff la" << std::endl;
            std::string lbl;
            for (const auto &pair : latch_EXE)
            {
                if (pair.first == "Label")
                {
                    std::cout << "found the label" << std::endl;
                    lbl = pair.second;
                }
            }
            if (labelToAddress.find(lbl) != labelToAddress.end())
            {
                // loaded_value=labelToAddress[lbl];
                reg[returnIndex(rd)] = labelToAddress[lbl];
            }
        }

        reg_state[returnIndex(rd)] = 3;
    }
    if (std::find(ins_type5.begin(), ins_type5.end(), opcode) != ins_type5.end())
    {
        if (opcode == "jalr")
        {
            reg[returnIndex(rd)] = PC + 1;
            // look into this
        }
    }
    if (opcode != "la" || opcode != "j")
    {
        latch_EXE.push_back({"result", std::to_string(result)});
    }
}
void Core::MemoryWF(std::vector<std::pair<std::string, std::string>> latch_EXE, char *RAM)
{
    std::cout << "MEM opcode" << std::endl;
    std::string rd;
    rd = search_latch("rd", latch_EXE);
    std::string opcode;
    latch_MEM = latch_EXE;
    for (const auto &pair : latch_EXE)
    {
        if (pair.first == "Opcode")
        {
            std::cout << "MEM opcode" << std::endl;
            opcode = pair.second;
        }
    }
    int result;

    for (const auto &pair : latch_EXE)
    {
        if (pair.first == "result")
        {

            result = stoi(pair.second);
            std::cout << result << std::endl;
        }
    }

    if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end() || std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
    {
        latch_MEM = latch_EXE;
        reg_state[returnIndex(rd)] = 4;
    }
    if (std::find(ins_type3.begin(), ins_type3.end(), opcode) != ins_type3.end())
    {
        //  executed_branch = true;
    }
    if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())

    {
        int loaded_value = 0;

        if (opcode == "lw")
        {

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
                    std::cout << "ff lw" << std::endl;
                }
            }
            std::cout << "succesful" << std::endl;
            reg_state[returnIndex(rd)] = 4;
            //  reg[returnIndex(rd)] = loaded_value;
        }
        else if (opcode == "sw")
        {
            std::cout << "ff sw" << std::endl;
            int load_value;
            for (const auto &pair : latch_EXE)
            {
                if (pair.first == "rd_value")
                {
                    std::cout << "I m the problem" << std::endl;
                    load_value = stoi(pair.second);
                }
            }

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
            std::cout << "ff la" << std::endl;
            std::string lbl;
            for (const auto &pair : latch_EXE)
            {
                if (pair.first == "Label")
                {
                    std::cout << "found the label" << std::endl;
                    lbl = pair.second;
                }
            }
            if (labelToAddress.find(lbl) != labelToAddress.end())
            {
                loaded_value = labelToAddress[lbl];
                reg[returnIndex(rd)] = labelToAddress[lbl];
            }
        }

        latch_MEM.push_back({"loaded_value", std::to_string(loaded_value)});
    }
    if (std::find(ins_type5.begin(), ins_type5.end(), opcode) != ins_type5.end())
    {
        executed_branch = true;
    }
}

void Core::WriteBackWF(std::vector<std::pair<std::string, std::string>> latch_MEM)
{

    std::string opcode = search_latch("Opcode", latch_MEM);

    std::string rd = search_latch("rd", latch_MEM);
    // std::cout<<opcode<<std::endl;
    if (std::find(ins_type1.begin(), ins_type1.end(), opcode) != ins_type1.end() || std::find(ins_type2.begin(), ins_type2.end(), opcode) != ins_type2.end())
    {
        int result = stoi(search_latch("result", latch_MEM));

        std::cout << "e" << std::endl;
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
            // reg_state[returnIndex(rd)] = 5;
        }
        if (opcode == "sw")
        {
        }
        if (opcode == "la")
        {
            std::string loaded_value = search_latch("loaded_value", latch_MEM);
            //  std::cout << "Val of ind" << loaded_value << std::endl;
            reg[returnIndex(rd)] = stoi(loaded_value);
            reg_state[returnIndex(rd)] = 5;

            std::cout << rd << std::endl;
        }
        reg_state[returnIndex(rd)] = 5;
    }
    if (std::find(ins_type4.begin(), ins_type4.end(), opcode) != ins_type4.end())
    {
        if (opcode == "jalr")
        {
            reg[returnIndex(rd)] = PC + 1;
            // look into this
        }
    }
}

void Core::Step_countWF(char *RAM)
{
    int latency_addi = latency_map["ADDI"] - 1;
    int latency_add = latency_map["ADD"] - 1;
    int latency_mul = latency_map["MUL"] - 1;
    int latency_sub = latency_map["SUB"] - 1;
    int c = 1;
    int y = 0, z = 0;
    int k = 0;
    while (keep_going)
    {
        y = 0;
        c++;
        if (latch_MEM.size() > 0)
        {
            k = 1;
            std::cout << "WB";
            for (const auto &pair : latch_MEM)
            {
                std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
            }
            pip[y + z][c] = "W";
            z++;
            WriteBackWF(latch_MEM);
            latch_MEM.clear();
        }
        if (latch_EXE.size() > 0)
        {

            k = 2;
            pip[y + z][c] = "M";
            y++;
            MemoryWF(latch_EXE, RAM);
            latch_EXE.clear();
            std::cout << "M";
        }
        if (latch_IDRF.size() > 0)
        {
            bool f = false;
            if (!stall_flag && !stall_flag2)
            {
                k = 3;
                std::cout << "E";
                std::string op = search_latch("Opcode", latch_IDRF);

                pip[y + z][c] = "E";
                y++;
                // std::cout << "the lat" << latency_addi << std::endl;
                if (op == "addi")
                {
                    if (latency_addi > 0)
                    {
                        f = true;
                        // std::cout << "latin" << std::endl;
                        latency_addi--;
                        continue;
                    }
                }
                else if (op == "add")
                {
                    if (latency_add > 0)
                    {
                        latency_add--;
                        continue;
                    }
                }
                else if (op == "sub")
                {
                    if (latency_sub > 0)
                    {
                        latency_sub--;
                        continue;
                    }
                }
                else if (op == "mul")
                {
                    if (latency_mul > 0)
                    {
                        latency_mul--;
                        continue;
                    }
                }
                if (f)
                {
                    continue;
                }
                ExecuteWF(latch_IDRF);
                if (executed_branch && mis_predict)
                {
                    PC = (findLabelIndex(search_latch("Label", latch_EXE))) + 1;
                    latch_IF.clear();
                    executed_branch = false;
                    mis_predict = false;
                }
                latency_addi = latency_map["ADDI"] - 1;
                latency_add = latency_map["ADD"] - 1;
                latency_mul = latency_map["MUL"] - 1;
                latency_sub = latency_map["SUB"] - 1;
                latch_IDRF.clear();
            }
        }
        if (latch_IF.size() > 0 || ishazard_notified)
        {
            if (eof && !deof)
            {
                if (latch_IDRF.size() == 0)
                {
                    k = 4;
                    pip[y + z][c] = "D";
                    y++;
                    DecodeWF();

                    std::cout << "ID";
                    latch_IF.clear();
                    deof = true;
                }
                else if ((stall_flag || stall_flag2) && latch_IDRF.size() == 2)
                {
                    k = 4;
                    pip[y + z][c] = "D";
                    y++;
                    DecodeWF();
                    deof = true;
                }
            }
            else
            {
                if (latch_IDRF.size() == 0)
                {
                    k = 4;
                    pip[y + z][c] = "D";
                    y++;
                    DecodeWF();

                    std::cout << "ID";
                    latch_IF.clear();
                }
                else if ((stall_flag || stall_flag2) && latch_IDRF.size() == 2)
                {
                    k = 4;
                    pip[y + z][c] = "D";
                    y++;
                    DecodeWF();

                    std::cout << "ID";
                }
            }
        }
        if (latch_IF.size() == 0 && !eof)
        {
            k = 5;
            pip[y + z][c] = "F";
            pip[y + z][0] = std::to_string(PC);
            y++;

            FetchWF();
            std::cout << "F";
        }
        else
        {
            std::cout << "came here but a stall;)" << std::endl;
        }
        std::cout << k << std::endl;
        if (v == 0)
            break;
        if (k == 0)
        {

            break;
            keep_going = 0;
        }
        else
        {
            loop++;
            std::cout << loop << std::endl;
        }
        std::cout << "\nONE cycle finish\n";
        v--;
    }
    std::cout << instructions.size() << std::endl;
    std::cout << stalls << std::endl;
    std::cout << loop << std::endl;
    std::cout << "WIth Forwarding" << std::endl;
    std::cout << "No of stalls" << std::endl;
    std::cout << stalls << std::endl;
    std::cout << "No of cycles" << std::endl;
    std::cout << loop << std::endl;
    std::cout << "Instructions per cycle" << std::endl;
    std::cout << stalls /(float) ins << std::endl;
    for (int i = 0; i < 30; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            std::cout << pip[i][j] << " ";
        }
        std::cout << std::endl;
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
