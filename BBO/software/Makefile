HOME = .
BIN = $(HOME)/bin
SRC = $(HOME)/src
INCLUDE = $(HOME)/include
LIB = $(HOME)/lib
OBJ = $(HOME)/obj


all: $(BIN)/BBO $(BIN)/AM fin 

$(OBJ)/utilidades.o: $(SRC)/utilidades.cpp 
	g++ -c $(SRC)/utilidades.cpp -o $(OBJ)/utilidades.o -I$(INCLUDE)

$(OBJ)/random.o:$(SRC)/random.cpp 
	g++ -c $(SRC)/random.cpp -o $(OBJ)/random.o -I$(INCLUDE)

$(OBJ)/BBO.o: $(SRC)/BBO.cpp $(INCLUDE)/utilidades.h  $(SRC)/random.cpp
	g++ -c $(SRC)/BBO.cpp -o $(OBJ)/BBO.o -I$(INCLUDE)

$(BIN)/BBO : $(OBJ)/BBO.o $(OBJ)/utilidades.o $(OBJ)/random.o
	g++ -o $(BIN)/BBO $(OBJ)/BBO.o $(OBJ)/utilidades.o $(OBJ)/random.o

$(OBJ)/AM.o: $(SRC)/AM.cpp $(INCLUDE)/utilidades.h  $(SRC)/random.cpp
	g++ -c $(SRC)/AM.cpp -o $(OBJ)/AM.o -I$(INCLUDE)

$(BIN)/AM : $(OBJ)/AM.o $(OBJ)/utilidades.o $(OBJ)/random.o
	g++ -o $(BIN)/AM $(OBJ)/AM.o $(OBJ)/utilidades.o $(OBJ)/random.o

clean :
	@echo
	@echo Borrando objetos
	-rm -rf obj/*
	@echo
	@echo Borrando ejecutables...
	-rm -rf bin/*

mr.proper : clean
	@echo
	@echo Borrando bibliotecas...
	-rm -rf lib/*
	@echo

fin: