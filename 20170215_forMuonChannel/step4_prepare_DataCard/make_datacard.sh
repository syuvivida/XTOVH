#!/bin/bash

# --------------------------------------
# (1) access signal name and yield
signal_mass_points=(800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000) 
signal_yield=(-99 -99 -99 -99 -99 -99 -99 -99 -99 -99 -99) # set dummpy value

for ((i=0; i<${#signal_mass_points[@]}; i++)); do
#  temp_name=`head -n $((i+2)) MuonCh_EvtNumber.txt | tail -1 | cut -d ':' -f 1` 
  signal_yield[$i]=`head -n $((i+2)) MuonCh_EvtNumber.txt | tail -1 | cut -d ':' -f 2`
#  echo $temp_name ${signal_yield[$i]}  
done 





# --------------------------------------
# (2) access data/background name and yield
data_name=`cat MuonCh_EvtNumber.txt  | grep "data_obs" | cut -d ':' -f 1`
data_yield=`cat MuonCh_EvtNumber.txt | grep "data_obs" | cut -d ':' -f 2`

background_name=`cat MuonCh_EvtNumber.txt  | grep "BACKGROUND" | cut -d ':' -f 1`
background_yield=`cat MuonCh_EvtNumber.txt | grep "BACKGROUND" | cut -d ':' -f 2`





# --------------------------------------
# (3) define function to write DataCard

function MakingDataCard(){

  # $1=${signal_mass_points[$i]}
  # $2=${signal_yield[$i]}
 
#  mass_point=`echo $1 | cut -d '_' -f 2`
  datacard_name=DataCard_and_ROOT_MuonCh/DataCard_ZPTOZHEE_13TeV_Run2016_M$1.txt
  signal_name=SIG_M$1

  echo $1 $2 $datacard_name 
  test -e $datacard_name  && rm -r $datacard_name  || echo " $datacard_name Not exist"

  # -------------------------------------------------------
  head -7 DataCard_EXAMPLE.txt 			   >> $datacard_name  # line 1-7

  temp1=`head -8 DataCard_EXAMPLE.txt |tail -1 | cut -d " " -f 15`
  temp2=`head -8 DataCard_EXAMPLE.txt |tail -1 | cut -d " " -f 17`
  echo "shapes  *      ZPTOZHMM     DataCard_and_ROOT_MuonCh/Shape_test.root  $temp1  $temp2" >> $datacard_name  # line 8

  head -12 DataCard_EXAMPLE.txt | tail -4          	   >> $datacard_name  # line 9-12
  echo "observation     $data_yield"               	   >> $datacard_name  # line 13
  head -17 DataCard_EXAMPLE.txt | tail -4          	   >> $datacard_name  # line 14-17
  echo "process          $signal_name    $background_name" >> $datacard_name  # line 18
  head -22 DataCard_EXAMPLE.txt | tail -4          	   >> $datacard_name  # line 19-22
  echo "rate             $2    $background_yield"  	   >> $datacard_name  # line 23
  head -36 DataCard_EXAMPLE.txt | tail -13         	   >> $datacard_name  # line 24-36
  # -------------------------------------------------------
}





# ------------------------------------------ 
# (4) write DataCard for each mass points

for ((i=0; i<${#signal_mass_points[@]}; i++)); do

#  echo ${signal_mass_points[$i]} ${signal_yield[$i]}  
   MakingDataCard ${signal_mass_points[$i]} ${signal_yield[$i]} 

done








