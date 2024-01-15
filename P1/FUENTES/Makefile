HOME = .
BIN = $(HOME)/bin
SRC = $(HOME)/src
INCLUDE = $(HOME)/include
LIB = $(HOME)/lib
OBJ = $(HOME)/obj


all: $(BIN)/main fin 

$(OBJ)/utilidades.o: $(SRC)/utilidades.cpp 
	g++ -c $(SRC)/utilidades.cpp -o $(OBJ)/utilidades.o -I$(INCLUDE)

$(OBJ)/greedy.o: $(SRC)/greedy.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/greedy.cpp -o $(OBJ)/greedy.o -I$(INCLUDE)

$(OBJ)/bl.o: $(SRC)/bl.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/bl.cpp -o $(OBJ)/bl.o -I$(INCLUDE)

$(OBJ)/main.o: $(SRC)/main.cpp $(INCLUDE)/greedy.h $(INCLUDE)/bl.h $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/main.cpp -o $(OBJ)/main.o -I$(INCLUDE)

$(BIN)/main : $(OBJ)/main.o $(OBJ)/greedy.o $(OBJ)/bl.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/main $(OBJ)/main.o $(OBJ)/greedy.o $(OBJ)/bl.o $(OBJ)/utilidades.o


	
clean :
	@echo
	@echo Borrando objetos
	-rm -rf obj/*
	@echo
	@echo Borrando ejecutables...
	-rm -rf bin/main

mr.proper : clean
	@echo
	@echo Borrando bibliotecas...
	-rm -rf lib/*
	@echo

fin: