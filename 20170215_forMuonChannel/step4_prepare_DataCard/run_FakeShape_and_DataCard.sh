#!/bin/bash

echo " "; echo "(1) prepare histogram for shape and print its normalization"; echo " "
root -l -q -b Fake_shape_prepare.C++ 

echo " "; echo "(2) make DataCard of each mass points"; echo " "
./make_datacard.sh

rm *.so *.d *.pcm
echo " "
