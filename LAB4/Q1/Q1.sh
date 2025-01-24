#!/bin/bash

# Prompt user to enter the file path
echo "Enter the path of the file to duplicate:"
read file_path

# Check if the file exists
if [ ! -f "$file_path" ]; then
    echo "Error: The specified file does not exist."
    exit 1
fi

# Get the file name and extension
file_name=$(basename "$file_path")
file_dir=$(dirname "$file_path")

# Create the duplicate file name (append _copy to the original file name)
duplicate_file="${file_dir}/${file_name%.*}_copy.${file_name##*.}"

# Copy the file
cp "$file_path" "$duplicate_file"

# Output the result
echo "Duplicate copy created: $duplicate_file"
