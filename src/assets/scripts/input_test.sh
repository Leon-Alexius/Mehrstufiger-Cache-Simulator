#!/bin/bash

# Initialize test status
test_status=0

: '
Redirecting stdout to /dev/null:

    1>/dev/null --> stdout is discarded.
    2>&1 --> stderr is captured into the output variable.
'

# Function to run a command and check for an expected error message
run_test() {
    echo "Testing: $1"
    output=$(eval $1 2>&1 1>/dev/null) # Ignore stdout and capture stderr
    if [[ "$output" == *"$2"* ]]; then
        if [[ "$2" == "" ]]; then
            echo "PASS: Expected behaviour received."
        else
            echo "PASS: Expected error received."
        fi
    else
        echo "FAIL: Expected error/behaviour not received."
        echo "Received: $output"
        test_status=1 # Mark test as failed
    fi
    echo "--------------------------------"
}

# Test: Order of arguments does not matter
run_test "./cache -c 100 -p false examples/ijk/ijk.csv" ""
run_test "./cache -p false -c 100 examples/ijk/ijk.csv" ""

# Test: Short and Long options
run_test "./cache -p false examples/ijk/ijk.csv" ""
run_test "./cache --pretty-print false examples/ijk/ijk.csv" ""

# Test: Shortest possible input valid
run_test "./cache examples/ijk/ijk.csv" ""

# Test: No arguments
run_test "./cache" "Invalid input: Filename is missing"

# Test: Filename has no extension
run_test "./cache examples/ijk/ijk" "Invalid filename. Filename should end with .csv"

# Test: filename (positional argument) is (not) the last argument
run_test "./cache examples/ijk/ijk.csv --cycles 10000" ""
run_test "./cache --cycles 10000 examples/ijk/ijk.csv" ""

# Test: Multiple Non-Optional Arguments
run_test "./cache examples/ijk/ijk_opt1.csv examples/ijk/ijk.csv" "Invalid input: examples/ijk/ijk_opt1.csv is not allowed here!"
run_test "./cache leon examples/ijk/ijk.csv" "Invalid input: leon is not allowed here!"

# Test: Case Sensitive
run_test "./cache -P false examples/ijk/ijk.csv" "./cache: invalid option -- 'P'"

# Test: No whitespace between option and argument
run_test "./cache -pfalse examples/ijk/ijk.csv" ""
run_test "./cache --pretty-printfalse examples/ijk/ijk.csv" ""

# Test: Incomplete optional arguments
run_test "./cache --tf=src/assets/vcd/def examples/ijk/ijk.csv" ""
run_test "./cache --t=src/assets/vcd/def examples/ijk/ijk.csv" ""
run_test "./cache -tsrc/assets/vcd/def examples/ijk/ijk.csv" "./cache: invalid option -- 't'"
run_test "./cache --m 100 examples/ijk/ijk.csv" ""
run_test "./cache -m 100 examples/ijk/ijk.csv" "./cache: invalid option -- 'm'"

# Test: help
run_test "./cache --help" ""
run_test "./cache -h" ""

# Exit with the overall test status
exit $test_status