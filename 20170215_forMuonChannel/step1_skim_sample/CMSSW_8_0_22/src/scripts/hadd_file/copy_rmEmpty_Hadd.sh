#!/bin/bash

scriptname=`basename $0`
EXPECTED_ARGS=1
if [ $# -ne $EXPECTED_ARGS ]
then
    echo "Usage: $scriptname Copy_Folder_Path"
    echo "Example: ./$scriptname ../skimmedFile_SingleMuon_Run2016B"
    exit 1
fi

echo "the path is $1"

# step1 copy
./copy.sh $1

# step2 delete empty ROOT file
./rm_1KFile.sh

# step3 hadd ROOT file 
Dataset_Name=`echo $1 | cut -d '/' -f 2`
./hadd.sh $Dataset_Name

