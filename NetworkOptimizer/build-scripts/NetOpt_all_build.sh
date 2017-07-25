#!/bin/bash

echo "-------------"
echo "Building AnyGraph-debug"
echo ""
cd ../../AnyGraph/Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building AnyGraph-debug.so"
echo ""
cd ../SharedLib-Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building AnyGraph-optimized.so"
echo ""
cd ../SharedLib-Optimized
make all -f makefile_not_IDE.mk

#########

echo "-------------"
echo "Building Simulation-debug"
echo ""
cd ../../Simulation/Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building Simulation-optimized"
echo ""
cd ../Optimized
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building Simulation-debug.so"
echo ""
cd ../SharedLib-Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building Simulation-optimized.so"
echo ""
cd ../SharedLib-Optimized
make all -f makefile_not_IDE.mk

#########

echo "-------------"
echo "Building NetworkOptimizer-debug"
echo ""
cd ../../NetworkOptimizer/Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building NetworkOptimizer-debug-test"
echo ""
cd ../Debug-Test
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building NetworkOptimizer-optimized"
echo ""
cd ../Optimized
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building NetworkOptimizer-debug.so"
echo ""
cd ../SharedLib-Debug
make all -f makefile_not_IDE.mk

echo "-------------"
echo "Building NetworkOptimizer-optimized.so"
echo ""
cd ../SharedLib-Optimized
make all -f makefile_not_IDE.mk

#########

echo "-------------"
echo "Finished building all NetworkOptimizer, please check output for possible errors (if any)."

