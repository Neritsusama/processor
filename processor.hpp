#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <string>
#include <sstream>
#include <cstdint>

class Processor {
private:
    // Fonctions utilitaires
    uint16_t saturate(int32_t value);
    bool is_valid_register(const std::string& reg);
    int get_register_index(const std::string& reg);
    uint8_t get_address_from_token(const std::string& token, uint16_t* registers);
    uint16_t get_value_from_token(const std::string& token, uint16_t* registers);
    
    // Fonctions d'exécution pour chaque instruction
    void execute_set(std::istringstream& iss, uint16_t* registers);
    void execute_add(std::istringstream& iss, uint16_t* registers);
    void execute_sub(std::istringstream& iss, uint16_t* registers);
    void execute_print(std::istringstream& iss, uint16_t* registers);
    bool execute_ifnz(std::istringstream& iss, uint16_t* registers);
    void execute_push(std::istringstream& iss, uint16_t* registers);
    void execute_pop(std::istringstream& iss, uint16_t* registers);
    void execute_store(std::istringstream& iss, uint16_t* registers);
    void execute_load(std::istringstream& iss, uint16_t* registers);
    bool execute_instruction(const std::string& line, uint16_t* registers);
    
    // Fonctions de la pile et mémoire
    void push(uint16_t value);
    uint16_t pop();
    void write(uint8_t address, uint16_t value);
    uint16_t read(uint8_t address);

public:
    void exec(const std::string& program_path);
};

#endif // PROCESSOR_HPP