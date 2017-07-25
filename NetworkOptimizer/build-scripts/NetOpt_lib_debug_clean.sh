#!/bin/bash

echo "-------------"
echo "Cleaning AnyGraph-debug.so"
echo ""
cd ../../AnyGraph/SharedLib-Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning Simulation-debug.so"
echo ""
cd ../../Simulation/SharedLib-Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning NetworkOptimizer-debug.so"
echo ""
cd ../../NetworkOptimizer/SharedLib-Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Finished cleaning NetworkOptimizer-debug.so, please check output for possible errors (if any)."

