#!/bin/bash


sample_name=SingleMuonRun2016BCDEFG_combine

root -l -q -b validate_data.C+\(\"$sample_name\"\) 
#root -l -q -b validate_data.C++ 

rm *.d *.so *.pcm




