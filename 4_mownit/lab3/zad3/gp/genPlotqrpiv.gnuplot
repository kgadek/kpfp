qrpiv(x)=a*x*x+b*x+c

fit qrpiv(x) 'gpData.in' using 1:8 via a,b,c

set xlabel "Rozmiar danych"
set ylabel "Czas"

set xrange [1:384]
set yrange [0.0001:10]


plot 'gpData.in' using 1:8:9:10 w yerrorbars title 'QR-PIV', qrpiv(x) title 'QR-PIV-aprox'
