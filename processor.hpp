#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>  // Inclusion des types entiers avec taille fixe
#include <string>   // Inclusion de la gestion des chaînes de caractères
#include <vector>   // Pour std::vector

// Définition de la classe Processor
class Processor {
public:
    /**
     * Applique une saturation sur une valeur 32 bits pour la ramener à un entier 16 bits.
     *
     * Args:
     *     value (int32_t) : Valeur d'entrée à saturer.
     * Return:
     *     uint16_t : Valeur saturée.
     */
    uint16_t saturate(int32_t value);

    /**
     * Analyse une instruction et retourne l'opcode sous forme de chaîne de caractères.
     *
     * Args:
     *     instr (std::string) : Instruction sous forme de chaîne.
     * Return:
     *     std::string : Opcode extrait de l'instruction.
     */
    std::string parse_opcode(const std::string& instr);

    /**
     * Extrait l'opérande d'une instruction sous forme d'entier 16 bits.
     *
     * Args:
     *     instr (std::string) : Instruction sous forme de chaîne.
     * Return:
     *     uint16_t : Opérande extrait.
     */
    uint16_t parse_operand(const std::string& instr);

    /**
     * Exécute un programme à partir d'un fichier donné.
     *
     * Args:
     *     program_path (std::string) : Chemin du fichier programme à exécuter.
     */
    void exec(const std::string& program_path);

    /**
     * Analyse une instruction et retourne ses composants sous forme de vecteur de strings.
     *
     * Args:
     *     instr (std::string) : Instruction à analyser.
     * Return:
     *     std::vector<std::string> : Vecteur contenant les tokens de l'instruction.
     */
    std::vector<std::string> parse_instruction(const std::string& instr);

    /**
     * Vérifie si un nom de registre est valide (a, b, c ou d).
     *
     * Args:
     *     reg (std::string) : Nom du registre à vérifier.
     * Return:
     *     bool : True si le registre est valide, false sinon.
     */
    bool is_valid_register(const std::string& reg);
};

#endif // PROCESSOR_HPP