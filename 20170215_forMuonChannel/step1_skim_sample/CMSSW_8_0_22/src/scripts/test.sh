#!/bin/bash

echo "0:" $0
echo "1:" $1
echo "2:" $2

aa=$1
bb=$2


while [ $aa -lt $bb  ];
do 

echo "aa:" $aa
echo "bb:" $bb

aa=$(( $aa + 1 ))

done






