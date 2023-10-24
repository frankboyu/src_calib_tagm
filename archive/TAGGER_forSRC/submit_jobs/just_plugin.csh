#!/bin/csh -f

# --------------------------------------------------------
#
# Generic script to run hd_root on cluster.
# Modify hd_root command to have plugin(s) of choice.
#
# --------------------------------------------------------
setenv ENVIRONMENT $1
setenv RUN_NUMBER $2
setenv OUTDIR $3

echo "starting........ "
date

echo "ENVIRONMENT  = $ENVIRONMENT"
echo "RUN_NUMBER   = $RUN_NUMBER"
echo "OUTDIR       = $OUTDIR"

#echo "df -h:"
#df -h
source $ENVIRONMENT
#setenv hdr /group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/halld_recon/halld_recon-4.20.0/Linux_CentOS7.7-x86_64-gcc4.8.5/bin/hd_root
#setenv HALLD_RECON_HOME /group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/halld_recon/halld_recon-4.20.0

set THREAD_TIMEOUT_VALUE = 300
set CALIB_CONTEXT_VALUE = ""

echo "starting plugins............"
date
set START = `date +%s`
hd_root *.evio -PPLUGINS=TAGH_timewalk,TAGM_TW -PNTHREADS=1 --config=/work/halld2/home/tjhague/batch_scripts/ana_config.cfg -PTAGMHit:DELTA_T_ADC_TDC_MAX=200-PTAGMHit:DELTA_T_ADC_TDC_MAX=200 -PTAGMHit:CUT_FACTOR=0
echo "ending plugins ............."
date
set END = `date +%s`
set TIMEDIFF = `expr $END - $START`
echo "TIMEDIFF: $TIMEDIFF"

# save ROOT output file
echo "Saving ROOT file"
mkdir -p -m 775 ${OUTDIR}/ROOT/${RUN_NUMBER}/
if (-e hd_root.root) then
	cp -v hd_root.root ${OUTDIR}/ROOT/${RUN_NUMBER}/hd_root_${RUN_NUMBER}.root
	chmod 664 ${OUTDIR}/ROOT/${RUN_NUMBER}/hd_root_${RUN_NUMBER}.root
endif

echo "ending job ............."
date

echo ""
echo ""
echo ""
echo ""

ls -R
