#!/bin/bash

while read run;
do
  echo $run
  jcache get /mss/halld/RunPeriod-2021-11/rawdata/Run0${run}/hd_rawdata_0${run}_000.evio
done < runs.dat
