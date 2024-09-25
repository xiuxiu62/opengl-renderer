#!python

import subprocess
import os

# Run CMake build command
cmake_build_command = 'cmake --build target'
subprocess.run(cmake_build_command, shell=True, check=True)

# Run the example executable
example_path = os.path.join('target', 'example')
subprocess.run(example_path, check=True)
