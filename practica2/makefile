SRC = src
BIN = bin
DAT = datos

all: $(BIN)/genera_tiempos

$(BIN)/genera_tiempos:  $(SRC)/genera_tiempos.cpp 
	g++ -o $@ $< -std=c++11

clean:
	rm $(BIN)/* 
