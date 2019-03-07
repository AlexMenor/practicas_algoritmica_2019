#!/bin/bash

echo "" >> salidamergesort.dat
i=1000

while [ $i -lt 100000 ]
do
	./mergesort $i >> salida.dat
	i=`expr $i + 1000`
done
