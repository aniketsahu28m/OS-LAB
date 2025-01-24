#!/bin/bash

# Check if there are command line arguments
if [ $# -eq 0 ]; then
    echo "No files specified. Please provide the files to delete."
    exit 1
fi

# Loop through each file passed as a command line argument
for file in "$@"; do
    # Check if the file exists
    if [ -f "$file" ]; then
        # Ask for confirmation to delete the file
        echo -n "Are you sure you want to delete $file? (y/n): "
        read confirm
        if [ "$confirm" == "y" ] || [ "$confirm" == "Y" ]; then
            rm -i "$file"  # Interactive removal
            echo "$file has been deleted."
        else
            echo "$file has not been deleted."
        fi
    else
        echo "$file does not exist."
    fi
done
