#!python

import subprocess
import shutil
import os

# Run CMake command
cmake_command = 'cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON . -B target'

subprocess.run(cmake_command, shell=True, check=True)

# Copy compile_commands.json to current directory
source_file = os.path.join('target', 'compile_commands.json')
destination_file = 'compile_commands.json'
shutil.copy2(source_file, destination_file)
