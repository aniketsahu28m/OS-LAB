#!/bin/bash

# Function to compare two strings (for sorting)
compare_strings() {
    if [[ "$1" < "$2" ]]; then
        return 0   # $1 is less than $2
    elif [[ "$1" > "$2" ]]; then
        return 1   # $1 is greater than $2
    else
        return 2   # $1 is equal to $2
    fi
}

# Bubble Sort function to sort the arguments
sort_arguments() {
    n=$#
    args=("$@")  # Copy command-line arguments into an array
    for ((i = 0; i < n; i++)); do
        for ((j = 0; j < n-i-1; j++)); do
            # Compare adjacent strings using compare_strings function
            compare_strings "${args[j]}" "${args[j+1]}"
            result=$?
            if [ $result -gt 0 ]; then
                # Swap the strings if out of order
                temp="${args[j]}"
                args[j]="${args[j+1]}"
                args[j+1]="$temp"
            fi
        done
    done
    # Output the sorted strings
    echo "${args[@]}"
}

# Check if command line arguments are provided
if [ $# -eq 0 ]; then
    echo "No strings provided. Please provide strings to sort."
    exit 1
fi

# Call the sort_arguments function
sorted_strings=$(sort_arguments "$@")

# Display the sorted result
echo "Sorted strings: $sorted_strings"
