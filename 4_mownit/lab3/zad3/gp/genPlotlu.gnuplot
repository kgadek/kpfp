lu(x)=a*x*x+b*x+c

fit lu(x) 'gpData.in' using 1:14 via a,b,c

set xlabel "Rozmiar danych"
set ylabel "Czas"

set xrange [1:384]
set yrange [0.0001:10]


plot 'gpData.in' using 1:14:15:16 w yerrorbars title 'LU', lu(x) title 'LU-aprox'
