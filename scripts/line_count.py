#!python

import os

def count_lines(file_path):
    with open(file_path, 'r') as file:
        return sum(1 for line in file)

def find_and_count_files(directory):
    total_lines = 0
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(('.h', '.cc')):
                file_path = os.path.join(root, file)
                lines = count_lines(file_path)
                total_lines += lines
    return total_lines

# Replace 'your_directory_path' with the path to the directory you want to search
directory_path = 'src'
total_lines = find_and_count_files(directory_path)
print(f"Total lines: {total_lines}")
