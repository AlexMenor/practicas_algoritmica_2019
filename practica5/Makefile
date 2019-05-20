all: viajanteByB

viajanteGreedy: viajanteByB.cpp
	g++ $< -o $@ -std=c++11

probar16: viajanteByB
	./$< datosTSP/ulysses16.tsp


debugging: viajanteByB.cpp
	g++ -g $< -o viajanteByB -std=c++11
