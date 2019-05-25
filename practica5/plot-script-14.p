# Set the output to a png file
set terminal png size 500,500
# The file we'll write to
set output 'ulysses14byb.png'
# The graphic title
set title 'Ulysses 14 TSP ByB'
#plot the graphic
plot "paraplotear" using 2:3 with lines title 'Ulysses14', "datosTSP/ulysses14.tsp" using 2:3 with points title 'Ciudades'