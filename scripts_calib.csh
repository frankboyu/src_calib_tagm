#!/bin/bash

while read run;
do
  echo $run
  python timing.py -b ${run} ${run} rf default
  root -b -l -q tw_tagm.C\(${run}\)
  rm resolution.root problem-channels.txt new_offsets.txt
  ccdb add PHOTON_BEAM/microscope/fadc_time_offsets -r ${run}-${run} /work/halld2/home/boyu/src_calib_tagm/results/results_calib/adc-offsets-${run}.txt
  ccdb add PHOTON_BEAM/microscope/tdc_time_offsets -r ${run}-${run} /work/halld2/home/boyu/src_calib_tagm/results/results_calib/tdc-offsets-${run}.txt
  ccdb add PHOTON_BEAM/microscope/tdc_timewalk_corrections -r ${run}-${run} /work/halld2/home/boyu/src_calib_tagm/results/results_calib/tw-corr-${run}.txt
done < runs.dat
