sing(x)=a*x*x+b*x+c

fit sing(x) 'gpData.in' using 1:5 via a,b,c

set xlabel "Rozmiar danych"
set ylabel "Czas"

set xrange [1:384]
set yrange [0.0001:10]


plot 'gpData.in' using 1:5:6:7 w yerrorbars title 'sing', sing(x) title 'sing-aprox'
