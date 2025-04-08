#include <cstdint>

// Déclaration des fonctions
uint16_t read(uint8_t address);
void write(uint8_t address, uint16_t value);
void push(uint16_t value);
uint16_t pop();