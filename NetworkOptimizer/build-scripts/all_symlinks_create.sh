#!/bin/bash

if [ "$EUID" -ne 0 ]
    then echo "Please run this script as root (using sudo)"
    exit
fi

libFolder=/usr/local/lib

cd ../..
ln -sf $(realpath AnyGraph/SharedLib-Debug/libAnyGraph-debug.so) $libFolder
ln -sf $(realpath AnyGraph/SharedLib-Optimized/libAnyGraph-optimized.so) $libFolder

ln -sf $(realpath Simulation/SharedLib-Debug/libSimulation-debug.so) $libFolder
ln -sf $(realpath Simulation/SharedLib-Optimized/libSimulation-optimized.so) $libFolder

ln -sf $(realpath NetworkOptimizer/SharedLib-Debug/libNetworkOptimizer-debug.so) $libFolder
ln -sf $(realpath NetworkOptimizer/SharedLib-Optimized/libNetworkOptimizer-optimized.so) $libFolder

echo "Finished creating sym links, please check output for possible errors (if any)."

