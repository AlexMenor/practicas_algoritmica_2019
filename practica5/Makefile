all: viajanteByB viajanteBack

viajanteGreedy: viajanteByB.cpp
	g++ $< -o $@ -std=c++11
	
viajanteBack: viajanteBack.cpp
	g++ $< -o $@ -std=c++11

probar16: viajanteByB
	./$< datosTSP/ulysses16.tsp
	./a.out datosTSP/ulysses16.tsp datosTSP/ulysses16.tsp-byb > paraplotear
	gnuplot plot-script-16.p
	display 'ulysses16byb.png'

probar14: viajanteByB
	./$< datosTSP/ulysses14.tsp
	./a.out datosTSP/ulysses14.tsp datosTSP/ulysses14.tsp-byb > paraplotear
	gnuplot plot-script-14.p
	display 'ulysses14byb.png'

probar10: viajanteByB
	./$< datosTSP/ulysses10.tsp
	./a.out datosTSP/ulysses10.tsp datosTSP/ulysses10.tsp-byb > paraplotear
	gnuplot plot-script-10.p
	display 'ulysses10byb.png'


debugging: viajanteByB.cpp
	g++ -g $< -o viajanteByB -std=c++11
