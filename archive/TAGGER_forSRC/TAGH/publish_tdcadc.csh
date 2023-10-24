#!/bin/csh -f

Run=$1

#set path_to_files=/mss/halld/RunPeriod-2021-08/rawdata/
set path_to_calib=TAGH_OUTPUT/${Run}/

#This should be a script that sets up your ccdb variables
#I'm not including it since you should have one that sets up your local ccdb
source setup.csh

#echo $run
#echo $JANA_CALIB_URL
#echo $CCDB_CONNECTION
ccdb add /PHOTON_BEAM/hodoscope/fadc_time_offsets -v default -r ${Run}-${Run} ${path_to_calib}offsets/fadc_time_offsets.txt
