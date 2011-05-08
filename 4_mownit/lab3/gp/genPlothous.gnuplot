hous(x)=a*x*x+b*x+c

fit hous(x) 'gpData.in' using 1:2 via a,b,c

set xlabel "Rozmiar danych"
set ylabel "Czas"

set xrange [1:384]
set yrange [0.0001:10]


plot 'gpData.in' using 1:2:3:4 w yerrorbars title 'HH', hous(x) title 'HH-aprox'
