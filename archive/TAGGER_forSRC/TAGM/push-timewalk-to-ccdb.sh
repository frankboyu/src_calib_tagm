#!/bin/bash

# This script pushes TAGM_TW calibrations to the CCDB

# $1 - run number (no leading 0 in the run number)
#variation should be default, removing that option

ccdb add PHOTON_BEAM/microscope/tdc_timewalk_corrections -r $1-$1 -v default tagm_tw_new_$1.txt
