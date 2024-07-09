#!/bin/bash

# Update the system
sudo apt-get update -y

# Install C and C++ compilers
sudo apt-get install -y gcc g++ 

# Install specific versions of gcc and g++
sudo apt-get install -y gcc-7 g++-7

# Set gcc and g++ to the specific versions
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7 

# Install SystemC 2.3.3
wget https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz
tar -xvf systemc-2.3.3.tar.gz
cd systemc-2.3.3/
mkdir objdir
cd objdir/
../configure
make
sudo make install
