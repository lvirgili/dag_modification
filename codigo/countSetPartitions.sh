#!/bin/sh

outFile=results
n=$(seq 1 10)

for i in $n; do
    /usr/bin/time -apo $outFile echo "$i $(./setPart $i | wc -l)" >> $outFile
done
