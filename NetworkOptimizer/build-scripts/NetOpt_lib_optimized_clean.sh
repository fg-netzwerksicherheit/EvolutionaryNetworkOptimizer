#!/bin/bash

echo "-------------"
echo "Cleaning AnyGraph-optimized.so"
echo ""
cd ../../AnyGraph/SharedLib-Optimized
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning Simulation-optimized.so"
echo ""
cd ../../Simulation/SharedLib-Optimized
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning NetworkOptimizer-optimized.so"
echo ""
cd ../../NetworkOptimizer/SharedLib-Optimized
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Finished cleaning NetworkOptimizer-optimized.so, please check output for possible errors (if any)."

