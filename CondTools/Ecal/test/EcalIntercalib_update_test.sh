#!/bin/sh
export LD_PRELOAD=$CMS_ORACLEOCCI_LIB
cmsRun ./src/CondTools/Ecal/python/updateIntercali_test.py
ret=$?
conddb --db EcalIntercalibConstants_test.db list EcalIntercalib_test
echo "return code is $ret"
exit $ret
