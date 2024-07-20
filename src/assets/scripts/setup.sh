#!/usr/bin/env bash

# Updates the list of available packages + versions
sudo apt-get update -y

# Install specific versions of gcc and g++
# https://gcc.gnu.org/projects/cxx-status.html - Default C++14 in GCC 6.1 up until GCC 10 (inclusive)
# https://gcc.gnu.org/onlinedocs/ - Default C17 in GCC 11.5
sudo apt-get install -y gcc-11 g++-10

# Set gcc to ver. 11 and g++ to ver. 10
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 60

# Check version of gcc and g++
gcc_version=$(gcc -dumpversion)
gpp_version=$(g++ -dumpversion)
if [[ "$gcc_version" != "11" || "$gpp_version" != "10" ]]; then
    echo "FAIL: gcc and g++ are not set correctly"
    exit 1
else
    echo "PASS: gcc and g++ are set correctly"
    exit 0
fi
