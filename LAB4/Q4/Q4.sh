#!/bin/bash

# Check if an input file is provided
if [ $# -lt 2 ]; then
    echo "Usage: $0 <option> <file>"
    echo "Options: -linecount, -wordcount, -charcount"
    exit 1
fi

# Get the option and file from command line arguments
option=$1
file=$2

# Check if the file exists
if [ ! -f "$file" ]; then
    echo "Error: File '$file' does not exist."
    exit 1
fi

# Process the option using a case statement
case $option in
    -linecount)
        # Count the number of lines in the file
        line_count=$(wc -l < "$file")
        echo "Line count: $line_count"
        ;;
    
    -wordcount)
        # Count the number of words in the file
        word_count=$(wc -w < "$file")
        echo "Word count: $word_count"
        ;;
    
    -charcount)
        # Count the number of characters in the file
        char_count=$(wc -m < "$file")
        echo "Character count: $char_count"
        ;;
    
    *)
        echo "Invalid option. Please use one of the following options:"
        echo "-linecount"
        echo "-wordcount"
        echo "-charcount"
        exit 1
        ;;
esac
