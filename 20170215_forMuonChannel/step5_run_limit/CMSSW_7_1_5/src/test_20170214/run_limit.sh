#!/bin/bash

# (0) do cmsenv first


# -----------------------------------------
# (1) define function to run limit

function RunLimit(){

  # $1=${signal_mass_points[$i]}

  echo " "
  echo "Mass Point: " $1 " GeV"
  echo " "
  combine DataCard_and_ROOT_MuonCh/DataCard_ZPTOZHEE_13TeV_Run2016_M$1.txt  -m $1 -M Asymptotic
  echo " "

  mv higgsCombineTest.Asymptotic* limit_result/

}



# -----------------------------------------
# (2) run limit for each mass point

signal_mass_points=(800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000)

for ((i=0; i<${#signal_mass_points[@]}; i++)); do

#  echo ${signal_mass_points[$i]}  
   RunLimit ${signal_mass_points[$i]} 

done





