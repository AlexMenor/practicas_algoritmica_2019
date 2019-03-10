SRC = src
BIN = bin
DAT = datos

all: $(BIN)/genera_tiempos

$(BIN)/genera_tiempos:  $(SRC)/genera_tiempos.cpp $(SRC)/funciones_del_profesor.cpp
	g++ -o $@ $< -std=c++11

clean:
	rm $(BIN)/* 
