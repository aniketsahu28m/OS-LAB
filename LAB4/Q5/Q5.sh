#!/bin/bash

# Function to search for patterns in the given file
search_patterns() {
    echo "Searching for patterns in the file..."
    for pattern in "$@"; do
        echo "Pattern: $pattern"
        grep -n "$pattern" "$file"  # Display all lines containing the pattern with line numbers
    done
}

# Function to delete patterns from the given file
delete_patterns() {
    echo "Deleting patterns from the file..."
    for pattern in "$@"; do
        sed -i "s/$pattern//g" "$file"  # Remove all occurrences of the pattern
        echo "All occurrences of '$pattern' have been deleted."
    done
}

# Menu-driven interface
menu() {
    while true; do
        echo "--------------------------------------"
        echo "Choose an operation:"
        echo "1. Search patterns in the file"
        echo "2. Delete patterns in the file"
        echo "3. Exit"
        echo "--------------------------------------"
        read -p "Enter your choice (1-3): " choice

        case $choice in
            1)
                search_patterns "$@"
                ;;
            2)
                delete_patterns "$@"
                ;;
            3)
                echo "Exiting the script."
                exit 0
                ;;
            *)
                echo "Invalid option. Please choose a valid option (1-3)."
                ;;
        esac
    done
}

# Check if a valid input file and patterns are provided
if [ $# -lt 2 ]; then
    echo "Usage: $0 <file> <pattern1> <pattern2> ..."
    echo "You must provide a file and at least one pattern to search or delete."
    exit 1
fi

file=$1
shift  # Shift the arguments so that patterns are passed to the functions

# Check if the file exists
if [ ! -f "$file" ]; then
    echo "Error: The file '$file' does not exist."
    exit 1
fi

# Call the menu function
menu "$@"
