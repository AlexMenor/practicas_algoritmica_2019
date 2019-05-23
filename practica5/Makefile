all: viajanteByB viajanteBack

viajanteGreedy: viajanteByB.cpp
	g++ $< -o $@ -std=c++11
	
viajanteBack: viajanteBack.cpp
	g++ $< -o $@ -std=c++11

probar16: viajanteByB
	./$< datosTSP/ulysses16.tsp
	./a.out datosTSP/ulysses16.tsp datosTSP/ulysses16.tsp-byb > paraplotear
	gnuplot plot-script.p
	display 'ulysses16byb.png'


debugging: viajanteByB.cpp
	g++ -g $< -o viajanteByB -std=c++11
