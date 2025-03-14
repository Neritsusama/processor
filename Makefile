# Définition du compilateur C++
CXX = g++

# Options de compilation :
# -Wall      : Active les avertissements classiques.
# -Wextra    : Active des avertissements supplémentaires.
# -Wpedantic : Renforce le respect du standard C++.
# -std=c++23 : Utilisation du standard C++23.
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++23

# Nom de l'exécutable final
EXEC = processor

# Script de test
TEST_SCRIPT = tests1.py

# Règle principale : création de l'exécutable
$(EXEC): processor.o main.o
	$(CXX) $(CXXFLAGS) -o $(EXEC) processor.o main.o  # Ajout des fichiers objets

# Compilation du fichier processor.cpp en fichier objet
processor.o: processor.cpp processor.hpp
	$(CXX) $(CXXFLAGS) -c processor.cpp -o processor.o

# Compilation du fichier main.cpp en fichier objet
main.o: main.cpp processor.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Exécution des tests
test: $(EXEC)
	python3 $(TEST_SCRIPT) ./$(EXEC)

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f *.o $(EXEC)
