#!/bin/python

import subprocess

# Run CMake build command
cmake_build_command = 'cmake --build target'

subprocess.run(cmake_build_command, shell=True, check=True)

