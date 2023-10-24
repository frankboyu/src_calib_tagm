source /group/halld/Software/build_scripts/gluex_env_boot_jlab.csh
gxenv $HALLD_VERSIONS/version_4.46.0.xml

setenv PYTHONPATH /group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/hdgeant4/hdgeant4-2.29.0/g4py:/group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/halld_recon/halld_recon-4.29.0/Linux_CentOS7.7-x86_64-gcc4.8.5/python2:/group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/rcdb/rcdb_0.06.00/python:/group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/ccdb/ccdb_1.06.07/python:/group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/ccdb/ccdb_1.06.07/python/ccdb/ccdb_pyllapi:/group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/root/root-6.08.06^bs221/lib:
setenv CCDB_USER boyu
setenv JANA_CALIB_URL "mysql://ccdb_user@hallddb.jlab.org/ccdb"
setenv CCDB_CONNECTION "mysql://ccdb_user@hallddb.jlab.org/ccdb"

setenv hdr /group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/halld_recon/halld_recon-4.30.0/Linux_CentOS7.7-x86_64-gcc4.8.5/bin/hd_root
setenv HALLD_RECON_HOME /group/halld/Software/builds/Linux_CentOS7.7-x86_64-gcc4.8.5/halld_recon/halld_recon-4.30.0
