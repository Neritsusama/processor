#include "memory.hpp"
#include <cstdint>
#include <iostream>
#include <cstdlib>

constexpr uint8_t B = 8; // Taille des adresses en bits
constexpr uint16_t MEMORY_SIZE = 1 << B; // 2^8 = 256 bytes
constexpr uint8_t STACK_SIZE = 16; // 16 bytes pour le stack
constexpr uint8_t STACK_START = MEMORY_SIZE - STACK_SIZE; // Le stack commence à l'adresse 240

uint8_t memory[MEMORY_SIZE] = {0};
uint8_t sp = STACK_START; // Stack Pointer initialisé au début du stack

uint16_t read(uint8_t address) {
    if (address >= MEMORY_SIZE - 1) { // -1 car on lit 2 bytes
        std::cerr << "Erreur: Adresse mémoire invalide pour lecture: " << static_cast<int>(address) << std::endl;
        std::exit(1);
    }
    // Lecture d'une valeur 16 bits (little-endian)
    return (memory[address + 1] << 8) | memory[address];
}

void write(uint8_t address, uint16_t value) {
    if (address >= MEMORY_SIZE - 1) { // -1 car on écrit 2 bytes
        std::cerr << "Erreur: Adresse mémoire invalide pour écriture: " << static_cast<int>(address) << std::endl;
        std::exit(1);
    }
    // Écriture d'une valeur 16 bits (little-endian)
    memory[address] = value & 0xFF;
    memory[address + 1] = (value >> 8) & 0xFF;
}

void push(uint16_t value) {
    // Vérifier s'il y a assez d'espace dans la pile avant de décrémenter
    if (sp <= STACK_START - STACK_SIZE + 1) { // +1 car on a besoin de 2 bytes
        std::cerr << "Erreur: Stack overflow" << std::endl;
        std::exit(1);
    }
    // Écrire la valeur puis décrémenter le pointeur de stack de 2 bytes
    write(sp - 2, value);
    sp -= 2;
}

uint16_t pop() {
    // Vérifier si la pile n'est pas vide
    if (sp >= STACK_START) {
        std::cerr << "Erreur: Stack underflow" << std::endl;
        std::exit(1);
    }
    // Lire la valeur puis incrémenter le pointeur de stack de 2 bytes
    uint16_t value = read(sp);
    sp += 2;
    return value;
}