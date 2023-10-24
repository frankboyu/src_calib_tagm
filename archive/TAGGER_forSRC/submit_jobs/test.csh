#!/bin/csh -fxs

setenv ENVIRONMENT $1
setenv RUN_NUMBER $2
setenv OUTDIR $3

echo "starting........ "
date

echo "ENVIRONMENT  = $ENVIRONMENT"
echo "RUN_NUMBER   = $RUN_NUMBER"
echo "OUTDIR       = $OUTDIR"

source $ENVIRONMENT
set THREAD_TIMEOUT_VALUE = 300
set CALIB_CONTEXT_VALUE = ""

echo "starting plugins............"
date
set START = `date +%s`
hd_root *.evio -PPLUGINS=TAGM_TW -PNTHREADS=4 -PTAGMHit:DELTA_T_ADC_TDC_MAX=200 -PTAGMHit:CUT_FACTOR=0
#hd_root /cache/halld/RunPeriod-2021-11/rawdata/Run090059/hd_rawdata_090059_000.evio  -PPLUGINS=TAGM_TW -PNTHREADS=4 -PTAGMHit:DELTA_T_ADC_TDC_MAX=200 -PTAGMHit:CUT_FACTOR=0 -PEVENTS_TO_KEEP=500  
echo "ending plugins ............."
date
set END = `date +%s`
set TIMEDIFF = `expr $END - $START`
echo "TIMEDIFF: $TIMEDIFF"

# save ROOT output file
echo "Saving ROOT file"
if (-e hd_root.root) then
	cp -v hd_root.root ${OUTDIR}/calib/hd_root_calib_0${RUN_NUMBER}.root
	chmod 664 ${OUTDIR}/calib/hd_root_calib_0${RUN_NUMBER}.root
endif

#mkdir -p -m 775 ${OUTDIR}/calib/${RUN_NUMBER}/

echo "ending job ............."
date

echo ""
echo ""
echo ""
echo ""

ls -R
