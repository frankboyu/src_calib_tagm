#:!/bin/csh -f

# SET INPUTS
setenv ENVIRONMENT $1
setenv INPUTFILE $2
setenv CONFIG_FILE $3
setenv OUTDIR $4
setenv RUN_NUMBER $5
setenv FILE_NUMBER $6
setenv TREE_NAME_STRING $7 #comma-separated

# PRINT INPUTS
echo "ENVIRONMENT       = $ENVIRONMENT"
echo "INPUTFILE         = $INPUTFILE"
echo "CONFIG_FILE       = $CONFIG_FILE"
echo "OUTDIR            = $OUTDIR"
echo "RUN_NUMBER        = $RUN_NUMBER"
echo "FILE_NUMBER       = $FILE_NUMBER"
echo "TREE_NAME_STRING  = $TREE_NAME_STRING"

# ENVIRONMENT
source $ENVIRONMENT
echo pwd = $PWD
printenv

# COPY INPUT FILE TO WORKING DIRECTORY
# This step is necessary since the cache files will be created as soft links in the current directory, and we want to avoid large I/O processes.
# We first copy the input file to the current directory, then remove the link.
#ls -l
#cp $INPUTFILE ./tmp_file
#rm -f $INPUTFILE
#mv tmp_file $INPUTFILE
#ls -l




# This two line in here are written as a way to use a custom sqlite and not to overload the group file server. This was suggested by Mark Ito on 25th March 2020

# We first copy the sqlite to the current working directory. Make sure to copy the sqlite to a location the cache


#setenv JANA_CALIB_URL "mysql://ccdb_user@hallddb.jlab.org/ccdb"

# RUN JANA
hd_root $INPUTFILE --config=$CONFIG_FILE

# RETURN CODE
set RETURN_CODE = $?
echo "Return Code = " $RETURN_CODE
if ($RETURN_CODE != 0) then
	exit $RETURN_CODE
endif

# save output histograms
mkdir -p -m 775 ${OUTDIR}/${RUN_NUMBER}/
if (-e hd_root.root) then
	cp -v hd_root.root ${OUTDIR}/${RUN_NUMBER}/hd_root_${RUN_NUMBER}_${FILE_NUMBER}.root
	chmod 664 ${OUTDIR}/${RUN_NUMBER}/hd_root_${RUN_NUMBER}_${FILE_NUMBER}.root
endif

endif
