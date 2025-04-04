# Définition du compilateur C++
CXX = g++

# Options de compilation
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++23

# Nom de l'exécutable final
EXEC = processor

# Script de test
TEST_SCRIPT = tests2.py

# Fichiers objets à générer
OBJS = processor.o main.o memory.o  # Ajout de memory.o

# Règle principale : création de l'exécutable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)  # Lien avec tous les .o

# Compilation de processor.cpp
processor.o: processor.cpp processor.hpp memory.hpp
	$(CXX) $(CXXFLAGS) -c processor.cpp -o processor.o

# Compilation de main.cpp
main.o: main.cpp processor.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Compilation de memory.cpp (NOUVEAU)
memory.o: memory.cpp memory.hpp
	$(CXX) $(CXXFLAGS) -c memory.cpp -o memory.o

# Tests
test: $(EXEC)
	python3 $(TEST_SCRIPT) ./$(EXEC)

# Nettoyage
clean:
	rm -f *.o $(EXEC)