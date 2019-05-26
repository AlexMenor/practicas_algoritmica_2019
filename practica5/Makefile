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
	
probar22: viajanteByB
	./$< datosTSP/ulysses22.tsp
	./a.out datosTSP/ulysses22.tsp datosTSP/ulysses22.tsp-byb > paraplotear
	gnuplot plot-script-22.p
	display 'ulysses22byb.png'

probarburma: viajanteByB
	./$< datosTSP/burma14.tsp
	./a.out datosTSP/burma14.tsp datosTSP/burma14.tsp-byb > paraplotear
	gnuplot plot-script-burma.p
	display 'burma14byb.png'
	
probarburmaback: viajanteBack
	./$< datosTSP/burma14.tsp
	./a.out datosTSP/burma14.tsp datosTSP/burma14.tsp-back > paraplotear
	gnuplot plot-script-burmaback.p
	display 'burma14back.png'
	
probar10back: viajanteBack
	./$< datosTSP/ulysses10.tsp
	./a.out datosTSP/ulysses10.tsp datosTSP/ulysses10.tsp-back > paraplotear
	gnuplot plot-script-10back.p
	display 'ulysses10back.png'
	
probar14back: viajanteBack
	./$< datosTSP/ulysses14.tsp
	./a.out datosTSP/ulysses14.tsp datosTSP/ulysses14.tsp-back > paraplotear
	gnuplot plot-script-14back.p
	display 'ulysses14back.png'
	
probar16back: viajanteBack
	./$< datosTSP/ulysses16.tsp
	./a.out datosTSP/ulysses16.tsp datosTSP/ulysses16.tsp-back > paraplotear
	gnuplot plot-script-16back.p
	display 'ulysses16back.png'


debugging: viajanteByB.cpp
	g++ -g $< -o viajanteByB -std=c++11
