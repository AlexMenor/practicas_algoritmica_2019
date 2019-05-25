# Set the output to a png file
set terminal png size 500,500
# The file we'll write to
set output 'burma14byb.png'
# The graphic title
set title 'Burma 14 TSP ByB'
#plot the graphic
plot "paraplotear" using 2:3 with lines title 'Burma 14', "datosTSP/burma14.tsp" using 2:3 with points title 'Ciudades'