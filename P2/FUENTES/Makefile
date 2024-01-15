HOME = .
BIN = $(HOME)/bin
SRC = $(HOME)/src
INCLUDE = $(HOME)/include
LIB = $(HOME)/lib
OBJ = $(HOME)/obj


all: $(BIN)/AGG-u $(BIN)/AGG-p $(BIN)/AGE-u $(BIN)/AGE-p $(BIN)/AM fin 

$(OBJ)/utilidades.o: $(SRC)/utilidades.cpp 
	g++ -c $(SRC)/utilidades.cpp -o $(OBJ)/utilidades.o -I$(INCLUDE)

$(OBJ)/AGG-u.o: $(SRC)/AGG-u.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/AGG-u.cpp -o $(OBJ)/AGG-u.o -I$(INCLUDE)

$(BIN)/AGG-u : $(OBJ)/AGG-u.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/AGG-u $(OBJ)/AGG-u.o $(OBJ)/utilidades.o

$(OBJ)/AGG-p.o: $(SRC)/AGG-p.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/AGG-p.cpp -o $(OBJ)/AGG-p.o -I$(INCLUDE)

$(BIN)/AGG-p : $(OBJ)/AGG-p.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/AGG-p $(OBJ)/AGG-p.o $(OBJ)/utilidades.o

$(OBJ)/AGE-u.o: $(SRC)/AGE-u.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/AGE-u.cpp -o $(OBJ)/AGE-u.o -I$(INCLUDE)

$(BIN)/AGE-u : $(OBJ)/AGE-u.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/AGE-u $(OBJ)/AGE-u.o $(OBJ)/utilidades.o

$(OBJ)/AGE-p.o: $(SRC)/AGE-p.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/AGE-p.cpp -o $(OBJ)/AGE-p.o -I$(INCLUDE)

$(BIN)/AGE-p : $(OBJ)/AGE-p.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/AGE-p $(OBJ)/AGE-p.o $(OBJ)/utilidades.o

$(OBJ)/AM.o: $(SRC)/AM.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/AM.cpp -o $(OBJ)/AM.o -I$(INCLUDE)

$(BIN)/AM : $(OBJ)/AM.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/AM $(OBJ)/AM.o $(OBJ)/utilidades.o

	
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