#!/bin/bash

for (( k = 0; $k < 5; k ++ )); do 
	for i in 0 508 4096 8188; do
		echo "size= $i _______";
		rm *.tmp;
		echo "Generowanie";
		/usr/bin/time -f "%E" ./testerS generuj $i 100 tmpA1.tmp;
		/usr/bin/time -f "%E" ./testerL generuj $i 100 tmpB1.tmp;
		cp tmpA1.tmp tmpA2.tmp;
		cp tmpB1.tmp tmpB2.tmp;
		echo "Stats";
		wc tmp*.tmp;
		echo "Sortowanie A";
		/usr/bin/time -f "%E" ./testerS sortuj tmpA1.tmp;
		/usr/bin/time -f "%E" ./testerL sortuj tmpA2.tmp;
		echo "Diff A"; diff tmpA1.tmp tmpA2.tmp;
		echo "Sortowanie B";
		/usr/bin/time -f "%E" ./testerS sortuj tmpB1.tmp;
		/usr/bin/time -f "%E" ./testerL sortuj tmpB2.tmp;
		echo "Diff B";
		diff tmpB1.tmp tmpB2.tmp;
	done
done
