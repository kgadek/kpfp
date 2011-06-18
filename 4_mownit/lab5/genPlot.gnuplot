#!/usr/bin/env gnuplot

!./real_life > gpData.in
plot 'gpData.in' using 1:2 title 'Drapieznicy' with lines, 'gpData.in' using 1:3 title 'Ofiary' with lines
set size 1.0, 0.6
set terminal postscript portrait enhanced color dashed lw 1 "Helvetica" 14 
set output "plot.ps"
replot
set terminal x11
set size 1,1
