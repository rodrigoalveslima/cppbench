#!/bin/bash

# Copyright (C) 2022 Georgia Tech Center for Experimental Research in Computer
# Systems

# This script formats C++ code according to the Google style standard.

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

# Format C++ header files.
for filepath in $(find -type f | grep ".*\.h$")
do
  clang-format-12 -i --style=google $filepath
done

# Format C++ files.
for filepath in $(find -type f | grep ".*\.cpp$")
do
  clang-format-12 -i --style=google $filepath
done
