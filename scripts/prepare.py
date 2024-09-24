#!/bin/python

import subprocess
import shutil
import os

# Run CMake command
cmake_command = 'cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ . -B target'

subprocess.run(cmake_command, shell=True, check=True)

# Copy compile_commands.json to current directory
source_file = os.path.join('target', 'compile_commands.json')
destination_file = 'compile_commands.json'
shutil.copy2(source_file, destination_file)
