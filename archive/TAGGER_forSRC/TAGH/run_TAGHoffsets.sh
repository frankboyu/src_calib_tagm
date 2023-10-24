RunNo=$1
OutputDir=TAGH_OUTPUT/${RunNo}
InputFile=$2
#cp -p ${InputFile} hd_root.root
mkdir -p offsets
ccdb dump /PHOTON_BEAM/hodoscope/fadc_time_offsets:${RunNo} > offsets/fadc_time_offsets_ccdb.txt
root -b -q 'tdcadc_manual.C("'${InputFile}'",16)'
root -b -q 'fadc_offsets.C("fits-csv")'
#rm -f hd_root.root
mkdir -p $OutputDir
mv offsets $OutputDir; mv fits-csv $OutputDir; mv *fit.pdf $OutputDir
