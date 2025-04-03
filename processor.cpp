#include "processor.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

uint16_t Processor::saturate(int32_t value) {
    if (value < 0) return 0;
    if (value > 65535) return 65535;
    return static_cast<uint16_t>(value);
}

std::vector<std::string> Processor::parse_instruction(const std::string& instr) {
    std::vector<std::string> tokens;
    std::istringstream iss(instr);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool Processor::is_valid_register(const std::string& reg) {
    return (reg == "a" || reg == "b" || reg == "c" || reg == "d");
}

void Processor::exec(const std::string& program_path) {
    std::ifstream file(program_path);
    std::string line;
    uint16_t registers[4] = {0}; // a=0, b=1, c=2, d=3
    bool skip_next = false;

    while (std::getline(file, line)) {
        if (skip_next) {
            skip_next = false;
            continue;
        }

        std::vector<std::string> tokens = parse_instruction(line);
        if (tokens.empty()) continue;

        std::string opcode = tokens[0];

        if (opcode == "SET" && tokens.size() >= 3) {
            std::string reg = tokens[1];
            if (!is_valid_register(reg)) continue;
            
            uint16_t value = static_cast<uint16_t>(std::stoul(tokens[2]));
            int reg_index = reg[0] - 'a';
            registers[reg_index] = saturate(value);
        } 
        else if (opcode == "ADD" && tokens.size() >= 3) {
            std::string reg_dest = tokens[1];
            if (!is_valid_register(reg_dest)) continue;
            
            int reg_dest_index = reg_dest[0] - 'a';
            
            if (is_valid_register(tokens[2])) {
                std::string reg_src = tokens[2];
                int reg_src_index = reg_src[0] - 'a';
                registers[reg_dest_index] = saturate(
                    static_cast<int32_t>(registers[reg_dest_index]) + 
                    static_cast<int32_t>(registers[reg_src_index])
                );
            } else {
                uint16_t value = static_cast<uint16_t>(std::stoul(tokens[2]));
                registers[reg_dest_index] = saturate(
                    static_cast<int32_t>(registers[reg_dest_index]) + 
                    static_cast<int32_t>(value)
                );
            }
        } 
        else if (opcode == "SUB" && tokens.size() >= 3) {
            std::string reg_dest = tokens[1];
            if (!is_valid_register(reg_dest)) continue;
            
            int reg_dest_index = reg_dest[0] - 'a';
            
            if (is_valid_register(tokens[2])) {
                std::string reg_src = tokens[2];
                int reg_src_index = reg_src[0] - 'a';
                registers[reg_dest_index] = saturate(
                    static_cast<int32_t>(registers[reg_dest_index]) - 
                    static_cast<int32_t>(registers[reg_src_index])
                );
            } else {
                uint16_t value = static_cast<uint16_t>(std::stoul(tokens[2]));
                registers[reg_dest_index] = saturate(
                    static_cast<int32_t>(registers[reg_dest_index]) - 
                    static_cast<int32_t>(value)
                );
            }
        } 
        else if (opcode == "PRINT" && tokens.size() >= 2) {
            std::string reg = tokens[1];
            if (!is_valid_register(reg)) continue;
            
            int reg_index = reg[0] - 'a';
            std::cout <<  registers[reg_index] <<  std::endl;
        } 
        else if (opcode == "IFNZ" && tokens.size() >= 2) {
            std::string reg = tokens[1];
            if (!is_valid_register(reg)) continue;
            
            int reg_index = reg[0] - 'a';
            if (registers[reg_index] == 0) {
                skip_next = true;
            }
        }
    }
}