#!/bin/bash

if [ "$EUID" -ne 0 ]
    then echo "Please run this script as root (using sudo)"
    exit
fi

libFolder="/usr/local/lib"

rm $libFolder/libAnyGraph-debug.so
rm $libFolder/libAnyGraph-optimized.so

rm $libFolder/libSimulation-debug.so
rm $libFolder/libSimulation-optimized.so

rm $libFolder/libNetworkOptimizer-debug.so
rm $libFolder/libNetworkOptimizer-optimized.so

echo "Finished removing sym links, please check output for possible errors (if any)."

