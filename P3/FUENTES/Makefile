HOME = .
BIN = $(HOME)/bin
SRC = $(HOME)/src
INCLUDE = $(HOME)/include
LIB = $(HOME)/lib
OBJ = $(HOME)/obj


all: $(BIN)/ES $(BIN)/BMB $(BIN)/ILS $(BIN)/ILS-ES fin 

$(OBJ)/utilidades.o: $(SRC)/utilidades.cpp 
	g++ -c $(SRC)/utilidades.cpp -o $(OBJ)/utilidades.o -I$(INCLUDE)

$(OBJ)/bl.o: $(SRC)/bl.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/bl.cpp -o $(OBJ)/bl.o -I$(INCLUDE)

$(OBJ)/ES.o: $(SRC)/ES.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/ES.cpp -o $(OBJ)/ES.o -I$(INCLUDE)

$(BIN)/ES : $(OBJ)/ES.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/ES $(OBJ)/ES.o $(OBJ)/utilidades.o

$(OBJ)/BMB.o: $(SRC)/BMB.cpp $(INCLUDE)/bl.h $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/BMB.cpp -o $(OBJ)/BMB.o -I$(INCLUDE)

$(BIN)/BMB : $(OBJ)/BMB.o $(OBJ)/bl.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/BMB $(OBJ)/BMB.o $(OBJ)/bl.o $(OBJ)/utilidades.o

$(OBJ)/ILS.o: $(SRC)/ILS.cpp $(INCLUDE)/bl.h $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/ILS.cpp -o $(OBJ)/ILS.o -I$(INCLUDE)

$(BIN)/ILS : $(OBJ)/ILS.o $(OBJ)/bl.o $(OBJ)/utilidades.o
	g++ -o $(BIN)/ILS $(OBJ)/ILS.o $(OBJ)/bl.o $(OBJ)/utilidades.o

$(OBJ)/ILS-ES.o: $(SRC)/ILS-ES.cpp $(INCLUDE)/utilidades.h
	g++ -c $(SRC)/ILS-ES.cpp -o $(OBJ)/ILS-ES.o -I$(INCLUDE)

$(BIN)/ILS-ES : $(OBJ)/ILS-ES.o  $(OBJ)/utilidades.o
	g++ -o $(BIN)/ILS-ES $(OBJ)/ILS-ES.o $(OBJ)/utilidades.o


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