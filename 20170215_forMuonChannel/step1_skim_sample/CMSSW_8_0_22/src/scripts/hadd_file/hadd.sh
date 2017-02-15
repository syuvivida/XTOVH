#!/bin/bash

echo "step3:hadd ROOT file"

echo "hadd $1_combine.root *.root"
hadd $1_combine.root *.root

mv *combine.root result
rm *.root
echo " "

