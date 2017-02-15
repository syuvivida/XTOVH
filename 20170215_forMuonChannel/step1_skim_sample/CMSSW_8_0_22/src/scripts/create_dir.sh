#!/bin/bash

# this script is used to create directory for each dataset

# please do these first
# (1) .L skimTree.C++ in ROOT
# (2) export X509_USER_PROXY=$HOME/private/grid.proxy, voms-proxy-init --voms cms

function create(){
 
  inputFile=$1
  echo "inputFile is $inputFile"

  new_dir=skimmedFile_$inputFile
  mkdir $new_dir

  cp $inputFile $new_dir
  cp launch_ntuple.sh runNtuple.sh skimTree* run.C $new_dir

  cd $new_dir 

  # send jobs to batch by nohup
  nohup sh ./launch_ntuple.sh $inputFile &

  cd $current

}

current=$PWD

# create directory

create SingleMuon_Run2016B
create SingleMuon_Run2016C
create SingleMuon_Run2016D
create SingleMuon_Run2016E
create SingleMuon_Run2016F
create SingleMuon_Run2016G
create SingleMuon_Run2016H

#create SingleMuon_Run2016B_v1
#create SingleMuon_Run2016B_v2
#create SingleMuon_Run2016B_v3



