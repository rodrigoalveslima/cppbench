#!/bin/bash

# Copyright (C) 2022 Georgia Tech Center for Experimental Research in Computer
# Systems

# This script compiles and runs the test program `test/test.cpp`. The execution
# logs can be found in `analysis/sample`.

# Change to the parent directory.
cd "$(dirname "$(dirname "$(readlink -fm "$0")")")"

# Process command-line arguments.
set -u
while [[ $# > 1 ]]; do
  case $1 in
    * )
      echo "Invalid argument: $1"
      exit 1
  esac
  shift
  shift
done

# Compile the test program.
g++ -o test/test test/test.cpp -Iinclude -pthread

# Run the test program.
./test/test "analysis/sample/"
