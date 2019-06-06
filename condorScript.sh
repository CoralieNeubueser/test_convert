#!/bin/zsh
workdir=`pwd`
MINICALODIR='/afs/cern.ch/work/c/cneubuse/prod/miniCalo2'

echo JOBSUB::START starting job in directory $workdir

cd $MINICALODIR
source env.sh
cd 'test_convert'
./convert /eos/user/c/cneubuse/miniCalo2/prod/maxGranular/$1_out.root /eos/user/c/cneubuse/miniCalo2/prod/maxGranular_converted/$1_out.root

exitstatus=$?
if [ $exitstatus != 0 ]
then
     echo JOBSUB::FAIL Geant failed with status $exitstatus
     
     exit $exitstatus
fi

exitstatus=$?
if [ $exitstatus != 0 ]
then
     echo JOBSUB::FAIL eos copy failed with status $exitstatus
else
     echo JOBSUB::SUCC job ended sucessfully
fi
exit $exitstatus
