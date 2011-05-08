#!/usr/bin/env python
# -*- coding: utf-8


#for datasize in 1 2 4 8 16 32 64 128 256; do
#    for method in hous sing qrpiv qr lu; do
#        for ((i=0;$i<10;i++)); do
#            echo "out/$datasize.$method.$i.out";
#            /usr/bin/time -o out/$datasize.$method.$i.out -f "%e" ./linsolve data/$datasize.in $method >/dev/null;
#        done;
#    done;
#done

def doJob():
    row = -1
    table = []
    for d in [1,2,4,8,16,32,64,128,256]:
        row += 1
        col = -1
        newrow = [d]                                        # X = ...
        for m in ["hous","sing","qrpiv","qr","lu"]:
            col += 1
            times = []
            for i in range(10):
                f = open("out/%d.%s.%d.out" % (d,m,i))
                for l in f: times.append(float(l))
                f.close()
            s=sum(times)/10.0                               # średnia
            dev=sum(map(lambda x: (x-s)**2, times))**0.5    # odchylenie standardowe
            newrow.extend([s, max(0,s-dev), s+dev])                # Y, Ylow, Yhigh
            print "%d.%s=%.3f" % (d,m,s/10)
        table.append(newrow)
    gf = open('gpData.in','w')
    gf.write("#N\t")
    for m in ["hous","sing","qrpiv","qr","lu"]:
        gf.write("%s\t%s_l\t%s_h\t" % (m,m,m))
    gf.write("\n")
    for row in table:
        gf.write("%d\t" % row[0])
        for y in row[1:]:
            gf.write("%.3f\t" % y)
        gf.write("\n")
    gf.close()
    return table

if __name__ == '__main__':
    doJob()