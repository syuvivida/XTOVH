#!/bin/bash

sample_path=/afs/cern.ch/work/y/yuchang/ZH_analysis_with_2016_data/CMSSW_8_0_8/src/Produce_MiniTTree/MiniTree_mu_20170213/skim_sample

macro=xAna_mu

outputFolder=output_mu

save_name=SingleMuonRun2016BCDEFG_combine

root -q -b -l $macro.C+\(\"$sample_path\"\,\"$outputFolder\"\,\"$save_name\"\)

rm inputdir.txt *.so *.d *.pcm



