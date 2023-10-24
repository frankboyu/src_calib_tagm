#!/bin/bash

while read run;
do
  echo $run
  root -b -l -q plot.C\(${run}\)
done < runs.dat
