#!/bin/bash

# Initialize test status
test_status=0

# Function to run a command and check for an expected error message
run_test() {
    echo "Testing: $1"
    output=$(eval $1 2>&1)
    if [[ "$output" == *"$2"* ]]; then
        echo "PASS: Expected error received."
    else
        echo "FAIL: Expected error not received."
        echo "Received: $output"
        test_status=1 # Mark test as failed
    fi
    echo "--------------------------------"
}

# Test 1: No arguments
run_test "./cache" "Invalid input: Filename is missing"

# Test 2: Filename has no extension
run_test "./cache examples/ijk/ijk" "Filename should be the last argument and ends with .csv"

# Test 3: filename is not at the end of the input
run_test "./cache examples/ijk/ijk.csv --cycles 10000" "Filename should be the last argument and ends with .csv"

# Test 4: Double filename (not implemented yet)
run_test "./cache examples/ijk/ijk_opt1.csv examples/ijk/ijk.csv" "Invalid input: examples/ijk/ijk_opt1.csv is not allowed here!"
run_test "./cache leon examples/ijk/ijk.csv" "Invalid input: leon is not allowed here!"

# Exit with the overall test status
exit $test_status