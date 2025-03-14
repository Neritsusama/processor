#include "processor.hpp"  // Inclut le fichier d'en-tête de la classe Processor
#include <iostream>       // Inclut la bibliothèque pour les entrées/sorties standard
#include <fstream>        // Inclut la bibliothèque pour la manipulation de fichiers
#include <algorithm>      // Inclut la bibliothèque pour les fonctions utilitaires comme std::min et std::max

// Fonction pour saturer une valeur entre 0 et 65535
uint16_t Processor::saturate(int32_t value) {
    if (value < 0) {
        return 0;  // Si la valeur est négative, retourne 0
    } else if (value > 65535) {
        return 65535;  // Si la valeur dépasse 65535, retourne 65535
    }
    return static_cast<uint16_t>(value);  // Sinon, retourne la valeur castée en uint16_t
}

// Fonction pour extraire l'opcode d'une instruction
std::string Processor::parse_opcode(const std::string& instr) {
    size_t space = instr.find(' ');  // Trouve la position de l'espace dans la ligne
    if (space == std::string::npos) {
        return instr;  // Si aucun espace n'est trouvé, retourne toute la ligne comme opcode
    }
    return instr.substr(0, space);  // Retourne la sous-chaîne avant l'espace (l'opcode)
}

// Fonction pour extraire l'opérande d'une instruction
uint16_t Processor::parse_operand(const std::string& instr) {
    size_t space = instr.find(' ');  // Trouve la position de l'espace dans la ligne
    if (space == std::string::npos) {
        return 0;  // Si aucun espace n'est trouvé, retourne 0 comme opérande
    }
    return static_cast<uint16_t>(std::stoul(instr.substr(space + 1)));  // Convertit la sous-chaîne après l'espace en uint16_t
}

// Fonction pour exécuter un programme à partir d'un fichier
void Processor::exec(const std::string& program_path) {
    std::ifstream file(program_path);  // Ouvre le fichier contenant le programme
    std::string line;  // Variable pour stocker chaque ligne du fichier
    uint16_t registre = 0;  // Registre de travail, initialisé à 0
    bool skip_next = false;  // Indicateur pour sauter la prochaine ligne

    // Boucle pour lire chaque ligne du fichier
    while (std::getline(file, line)) {
        if (skip_next) {
            skip_next = false;  // Réinitialise l'indicateur après avoir sauté une ligne
            continue;  // Passe à la prochaine itération de la boucle
        }

        // Extraire l'opcode de la ligne
        std::string opcode = parse_opcode(line);
        // Extraire l'opérande de la ligne (s'il y en a un)
        uint16_t operand = parse_operand(line);

        // Exécuter l'instruction en fonction de l'opcode
        if (opcode == "SET") {
            registre = saturate(operand);  // Définir le registre à la valeur de l'opérande
        } else if (opcode == "ADD") {
            // Ajouter l'opérande au registre et appliquer la saturation
            registre = saturate(static_cast<int32_t>(registre) + static_cast<int32_t>(operand));
        } else if (opcode == "SUB") {
            // Soustraire l'opérande du registre et appliquer la saturation
            registre = saturate(static_cast<int32_t>(registre) - static_cast<int32_t>(operand));
        } else if (opcode == "PRINT") {
            // Afficher la valeur actuelle du registre
            std::cout << registre << std::endl;
        } else if (opcode == "IFNZ") {
            // Si le registre est égal à 0, sauter la prochaine ligne
            if (registre == 0) {
                skip_next = true;  // Activer l'indicateur pour sauter la prochaine ligne
            }
        }
    }
}