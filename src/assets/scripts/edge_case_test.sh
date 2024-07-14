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

# Test 1: L1 cache size greater than L2 cache size
run_test "./cache -c 2147483646 --l1-lines 256 --l2-lines 64 examples/ijk.csv" "L1 cache lines count is greater than L2 cache lines count"

# Test 2: L1 latency greater than L2 latency or L2 latency greater than memory latency
run_test "./cache -c 2147483646 --l1-latency 13 --l2-latency 12 --memory-latency 100 examples/ijk.csv" "L1 latency is greater than L2 latency"
run_test "./cache -c 2147483646 --l1-latency 4 --l2-latency 101 --memory-latency 100 examples/ijk.csv" "L2 latency is greater than memory latency"

# Test 3: Cache lines or cache line size set to 0
run_test "./cache -c 2147483646 --l1-lines 0 --l2-lines 256 examples/ijk.csv" "L1 cache lines, L2 cache lines or cache line size is set to 0"
run_test "./cache -c 2147483646 --l1-lines 64 --l2-lines 0 examples/ijk.csv" "L1 cache lines count is greater than L2 cache lines count"
run_test "./cache -c 2147483646 --cacheline-size 0 examples/ijk.csv" "L1 cache lines, L2 cache lines or cache line size is set to 0"

# Test 4: Cycles to simulate is less than 0
run_test "./cache --cycles -1 examples/ijk.csv" "Cycles to simulate is less than 0"

# Make the script executable with chmod +x edge_cases_tests.sh
# Run the script with ./edge_cases_tests.sh

# Exit with the overall test status
exit $test_status