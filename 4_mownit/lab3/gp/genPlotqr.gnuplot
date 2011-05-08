qr(x)=a*x*x+b*x+c

fit qr(x) 'gpData.in' using 1:11 via a,b,c

set xlabel "Rozmiar danych"
set ylabel "Czas"

set xrange [1:384]
set yrange [0.0001:10]


plot 'gpData.in' using 1:11:12:13 w yerrorbars title 'QR', qr(x) title 'QR-aprox'
