#!/bin/bash

echo "-------------"
echo "Building AnyGraph-debug.so"
echo ""
cd ../../AnyGraph/SharedLib-Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building Simulation-debug.so"
echo ""
cd ../../Simulation/SharedLib-Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building NetworkOptimizer-debug.so"
echo ""
cd ../../NetworkOptimizer/SharedLib-Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Finished building NetworkOptimizer-debug.so, please check output for possible errors (if any)."

