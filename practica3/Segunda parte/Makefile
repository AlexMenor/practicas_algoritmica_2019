all: viajanteGreedy

viajanteGreedy: viajanteGreedy.cpp
	g++ $< -o $@ -std=c++11

probar16: viajanteGreedy
	./viajanteGreedy datosTSP/ulysses16.tsp

probar1000: viajanteGreedy
	./viajanteGreedy datosTSP/pr1002.tsp

debugging: viajanteGreedy.cpp
	g++ -g $< -o viajanteGreedy -std=c++11
