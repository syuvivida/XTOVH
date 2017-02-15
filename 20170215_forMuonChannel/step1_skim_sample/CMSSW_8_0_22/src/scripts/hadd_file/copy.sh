#!/bin/bash

scriptname=`basename $0`
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
    echo "Usage: $scriptname Copy_Folder_Path"
    echo "Example: ./$scriptname ../skimmedFile_SingleMuon_Run2016B"
    exit 1
fi

echo "step1:Copy"
echo "cp $1/*.root ."
cp $1/*.root .



