#!/usr/bin/env bash

rm -f rbs.dat

gcc rbs.c -lm -o rbs

#time ./rbs

./rbs | gnuplot

#gnuplot -p rbs.plot
