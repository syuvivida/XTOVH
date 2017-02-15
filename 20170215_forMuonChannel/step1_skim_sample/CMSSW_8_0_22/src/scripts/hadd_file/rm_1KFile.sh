#!/bin/bash

echo "step2: delete empty ROOT file"

# ----------------------------
echo "  first stage: delete file with 1.0K"

du -h skimmed_NCUGlobalTuples_* | grep  "1\.0K" > test.txt

inputfile=test.txt

iteration=0
lastfile=`cat $inputfile | wc -l`

while [ $iteration -lt $lastfile ]; 
do
  iteration=$(( iteration + 1 ))
  input=`head -n $iteration $inputfile  | tail -1 | awk '{print $2 }'`

#  echo $iteration $input

  if [ -s $PWD/$input ]
  then 
#    echo "file $input exist"
     rm $input
  fi 

done

rm test.txt

# ------------------------------------------
echo "  second stage: delete file with absolute zero size"


du -h skimmed_NCUGlobalTuples_*  > test2.txt

inputfile=test2.txt

iteration=0
lastfile=`cat $inputfile | wc -l`

while [ $iteration -lt $lastfile ]; 
do
  iteration=$(( iteration + 1 ))
  input=`head -n $iteration $inputfile  | tail -1 | awk '{print $1 }'`
  zero_file=`head -n $iteration $inputfile  | tail -1 | awk '{print $2 }'`

  if [ "$input" == "0" ]
  then
#      echo "find " $iteration $input
       rm $zero_file 
  fi


done

rm test2.txt




















