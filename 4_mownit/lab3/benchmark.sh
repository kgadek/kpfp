#!/bin/bash

for datasize in 1 2 4 8 16 32 64 128 256; do
	for method in hous sing qrpiv qr lu; do
		for ((i=0;$i<10;i++)); do
			echo "out/$datasize.$method.$i.out";
			/usr/bin/time -o out/$datasize.$method.$i.out -f "%e" ./linsolve data/$datasize.in $method >/dev/null;
		done;
	done;
done