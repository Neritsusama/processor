#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
#include <string>
#include <vector>

class Processor {
public:
    static uint16_t saturate(int32_t value);
    static std::vector<std::string> parse_instruction(const std::string& instr);
    static bool is_valid_register(const std::string& reg);
    void exec(const std::string& program_path);
};

#endif // PROCESSOR_HPP