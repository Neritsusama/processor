#include "processor.hpp"
#include "memory.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Constantes
constexpr uint16_t MAX_VALUE = 65535;
constexpr uint16_t MIN_VALUE = 0;
constexpr size_t NUM_REGISTERS = 4;
constexpr char REGISTER_BASE = 'a';

uint16_t Processor::saturate(int32_t value) {
    if (value < MIN_VALUE) return MIN_VALUE;
    if (value > MAX_VALUE) return MAX_VALUE;
    return static_cast<uint16_t>(value);
}

bool Processor::is_valid_register(const std::string& reg) {
    return (reg == "a" || reg == "b" || reg == "c" || reg == "d");
}

int Processor::get_register_index(const std::string& reg) {
    return reg[0] - REGISTER_BASE;
}

uint8_t Processor::get_address_from_token(const std::string& token, uint16_t* registers) {
    if (is_valid_register(token)) {
        int reg_index = get_register_index(token);
        return static_cast<uint8_t>(registers[reg_index]);
    } else {
        return static_cast<uint8_t>(std::stoul(token));
    }
}

uint16_t Processor::get_value_from_token(const std::string& token, uint16_t* registers) {
    if (is_valid_register(token)) {
        int reg_index = get_register_index(token);
        return registers[reg_index];
    } else {
        return static_cast<uint16_t>(std::stoul(token));
    }
}

void Processor::execute_set(std::istringstream& iss, uint16_t* registers) {
    std::string reg;
    std::string value_str;
    
    if (!(iss >> reg) || !is_valid_register(reg) || !(iss >> value_str)) {
        return;
    }
    
    int reg_index = get_register_index(reg);
    uint16_t value = static_cast<uint16_t>(std::stoul(value_str));
    registers[reg_index] = saturate(value);
}

void Processor::execute_add(std::istringstream& iss, uint16_t* registers) {
    std::string reg_dest;
    std::string operand;
    
    if (!(iss >> reg_dest) || !is_valid_register(reg_dest) || !(iss >> operand)) {
        return;
    }
    
    int reg_dest_index = get_register_index(reg_dest);
    uint16_t value = get_value_from_token(operand, registers);
    
    registers[reg_dest_index] = saturate(
        static_cast<int32_t>(registers[reg_dest_index]) + 
        static_cast<int32_t>(value)
    );
}

void Processor::execute_sub(std::istringstream& iss, uint16_t* registers) {
    std::string reg_dest;
    std::string operand;
    
    if (!(iss >> reg_dest) || !is_valid_register(reg_dest) || !(iss >> operand)) {
        return;
    }
    
    int reg_dest_index = get_register_index(reg_dest);
    uint16_t value = get_value_from_token(operand, registers);
    
    registers[reg_dest_index] = saturate(
        static_cast<int32_t>(registers[reg_dest_index]) - 
        static_cast<int32_t>(value)
    );
}

void Processor::execute_print(std::istringstream& iss, uint16_t* registers) {
    std::string reg;
    
    if (!(iss >> reg) || !is_valid_register(reg)) {
        return;
    }
    
    int reg_index = get_register_index(reg);
    std::cout << registers[reg_index] << std::endl;
}

bool Processor::execute_ifnz(std::istringstream& iss, uint16_t* registers) {
    std::string reg;
    
    if (!(iss >> reg) || !is_valid_register(reg)) {
        return false;
    }
    
    int reg_index = get_register_index(reg);
    return registers[reg_index] == 0;
}

void Processor::execute_push(std::istringstream& iss, uint16_t* registers) {
    std::string reg;
    
    if (!(iss >> reg) || !is_valid_register(reg)) {
        return;
    }
    
    int reg_index = get_register_index(reg);
    ::push(registers[reg_index]);  
}


void Processor::execute_pop(std::istringstream& iss, uint16_t* registers) {
    std::string reg;
    
    if (!(iss >> reg) || !is_valid_register(reg)) {
        return;
    }
    
    int reg_index = get_register_index(reg);
    registers[reg_index] = ::pop();  
}


void Processor::execute_store(std::istringstream& iss, uint16_t* registers) {
    std::string addr_token;
    std::string reg_value;
    
    if (!(iss >> addr_token) || !(iss >> reg_value) || !is_valid_register(reg_value)) {
        return;
    }
    
    uint8_t address = get_address_from_token(addr_token, registers);
    int reg_value_index = get_register_index(reg_value);
    ::write(address, registers[reg_value_index]);  
}

void Processor::execute_load(std::istringstream& iss, uint16_t* registers) {
    std::string addr_token;
    std::string reg_dest;
    
    if (!(iss >> addr_token) || !(iss >> reg_dest) || !is_valid_register(reg_dest)) {
        return;
    }
    
    uint8_t address = get_address_from_token(addr_token, registers);
    int reg_dest_index = get_register_index(reg_dest);
    registers[reg_dest_index] = ::read(address);  
}

bool Processor::execute_instruction(const std::string& line, uint16_t* registers) {
    std::istringstream iss(line);
    std::string opcode;
    
    if (!(iss >> opcode)) {
        return false; // Pas d'instruction à sauter
    }

    if (opcode == "SET") {
        execute_set(iss, registers);
    } 
    else if (opcode == "ADD") {
        execute_add(iss, registers);
    } 
    else if (opcode == "SUB") {
        execute_sub(iss, registers);
    } 
    else if (opcode == "PRINT") {
        execute_print(iss, registers);
    } 
    else if (opcode == "IFNZ") {
        return execute_ifnz(iss, registers);
    }
    else if (opcode == "PUSH") {
        execute_push(iss, registers);
    } 
    else if (opcode == "POP") {
        execute_pop(iss, registers);
    }
    else if (opcode == "STORE") {
        execute_store(iss, registers);
    }
    else if (opcode == "LOAD") {
        execute_load(iss, registers);
    }
    
    return false; // Pas d'instruction à sauter par défaut
}

void Processor::exec(const std::string& program_path) {
    std::ifstream file(program_path);
    std::string line;
    uint16_t registers[NUM_REGISTERS] = {0}; // a=0, b=1, c=2, d=3
    bool skip_next = false;

    while (std::getline(file, line)) {
        if (skip_next) {
            skip_next = false;
        } else {
            skip_next = execute_instruction(line, registers);
        }
    }
}