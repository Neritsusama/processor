#include "processor.hpp"  // Inclusion de l'en-tête du processeur
#include <iostream>        // Inclusion de la bibliothèque standard pour l'affichage

int main(int argc, char* argv[]) {
    // Vérification du nombre d'arguments passés au programme
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <program_file>" << std::endl;
        return 1;  // Retourne une erreur si l'argument est incorrect
    }

    Processor processor;   // Création d'une instance de la classe Processor
    processor.exec(argv[1]);  // Exécute le programme en passant le fichier en argument

    return 0;  // Fin du programme avec succès
}
