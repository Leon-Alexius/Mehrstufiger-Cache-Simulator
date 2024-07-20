#!/bin/bash

# Initialize test status
test_status=0

# Function to run a command and check for an expected error message
run_test() {
    echo "Testing: $1"
    output=$(eval $1 2>&1)
    if [[ "$output" == *"$2"* ]]; then
        echo "PASS: Expected error/ behaviour received."
    else
        echo "FAIL: Expected error/ behaviour not received."
        echo "Received: $output"
        test_status=1 # Mark test as failed
    fi
    echo "--------------------------------"
}

# Test 1: No arguments
run_test "./cache" "Invalid input: Filename is missing"

# Test 2: Filename has no extension
run_test "./cache examples/ijk/ijk" "Invalid filename. Filename should end with .csv"

# Test 3: filename (positional argument) is (not) the last argument
run_test "./cache examples/ijk/ijk.csv --cycles 10000" ""
run_test "./cache --cycles 10000 examples/ijk/ijk.csv" ""

# Test 4: Multiple Non-Optional Arguments
run_test "./cache examples/ijk/ijk_opt1.csv examples/ijk/ijk.csv" "Invalid input: examples/ijk/ijk_opt1.csv is not allowed here!"
run_test "./cache leon examples/ijk/ijk.csv" "Invalid input: leon is not allowed here!"

# Test 5: Case Sensitive
run_test "./cache -P false examples/ijk/ijk.csv" "./cache: invalid option -- 'P'"

# Test 6: No whitespace between option and argument
run_test "./cache -pfalse examples/ijk/ijk.csv" ""

# Exit with the overall test status
exit $test_status