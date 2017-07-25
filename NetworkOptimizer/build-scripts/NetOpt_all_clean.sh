#!/bin/bash

echo "-------------"
echo "Cleaning AnyGraph-debug"
echo ""
cd ../../AnyGraph/Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning AnyGraph-debug.so"
echo ""
cd ../SharedLib-Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning AnyGraph-optimized.so"
echo ""
cd ../SharedLib-Optimized
make clean -f makefile_not_IDE.mk

#########

echo "-------------"
echo "Cleaning Simulation-debug"
echo ""
cd ../../Simulation/Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning Simulation-optimized"
echo ""
cd ../Optimized
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning Simulation-debug.so"
echo ""
cd ../SharedLib-Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning Simulation-optimized.so"
echo ""
cd ../SharedLib-Optimized
make clean -f makefile_not_IDE.mk

#########

echo "-------------"
echo "Cleaning NetworkOptimizer-debug"
echo ""
cd ../../NetworkOptimizer/Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning NetworkOptimizer-debug-test"
echo ""
cd ../Debug-Test
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning NetworkOptimizer-optimized"
echo ""
cd ../Optimized
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning NetworkOptimizer-debug.so"
echo ""
cd ../SharedLib-Debug
make clean -f makefile_not_IDE.mk

echo "-------------"
echo "Cleaning NetworkOptimizer-optimized.so"
echo ""
cd ../SharedLib-Optimized
make clean -f makefile_not_IDE.mk

#########

echo "-------------"
echo "Finished cleaning all NetworkOptimizer, please check output for possible errors (if any)."

