SRC = src
OBJ = obj
BIN = bin


all: $(BIN)/genera_tiempos

$(BIN)/genera_tiempos: $(OBJ)/funciones_del_profesor.o

$(OBJ)/funciones_del_profesor.o: $(SRC)/funciones_del_profesor.cpp
	g++ -c $< -o $@

clean:
	rm $(OBJ)/* $(BIN)/*