#!/bin/bash

echo "-------------"
echo "Building AnyGraph-optimized.so"
echo ""
cd ../../AnyGraph/SharedLib-Optimized
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building Simulation-optimized.so"
echo ""
cd ../../Simulation/SharedLib-Optimized
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building NetworkOptimizer-optimized.so"
echo ""
cd ../../NetworkOptimizer/SharedLib-Optimized
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Finished building NetworkOptimizer-optimized.so, please check output for possible errors (if any)."

