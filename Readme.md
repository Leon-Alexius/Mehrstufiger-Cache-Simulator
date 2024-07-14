# Cache Simulation Project

## Overview
This project aims to investigate the impact of multi-level caches on runtime and latency. It involves researching cache sizes, architectures, and latencies in modern processors, and implementing a cache simulation in SystemC with C++ and a framework program in C.

## Requirements

Ensure you have the following software installed on your system:

| Language | Version |
|----------|---------|
| C        | C17     |
| C++      | C++14   |
| SystemC  | 2.3.3   |

More Details:
- C Compiler (GCC recommended) supporting C17 standard
- C++ Compiler (G++ recommended) supporting C++14 standard
- SystemC library version 2.3.3

If you do not have these prerequisites installed, please refer to the respective documentation for installation instructions.

## Installation

1. **Clone the Repository**: Begin by cloning the project repository to your local machine. Open a terminal and execute the following command:
`git clone https://github.com/Dark-Rosaleen/GRA-Abschlussprojekt.git`

2. **Verify Installation of Prerequisites**: Ensure that the required versions of C, C++, and SystemC are installed on your system. If you are unsure or need to install the prerequisites, run the provided `setup.sh` script by executing: `./setup.sh`
This script will guide you through the installation of any missing prerequisites.

3. **Build the Project**: With the prerequisites in place, compile the project using the `make` command. In the terminal, navigate to the project directory and execute: `make`

## Running the Simulation

After successfully compiling the project, you can run the cache simulation with the following command: `./cache [FLAG(s)] filename.csv`

Replace `[FLAG(s)]` with any flags you wish to use during the simulation and `filename.csv` with the path to your input CSV file containing the simulation requests.

### List of Available Flags

| Flag              | Description                                                        | Default Value      |
|-------------------|--------------------------------------------------------------------|--------------------|
| `-c`, `--cycles`  | The number of cycles to be simulated                               | `1000000`          |
| `--cacheline-size`| The size of a cache line in bytes                                  | `64`               |
| `--l1-lines`      | The number of cache lines of the L1 cache                          | `64`               |
| `--l2-lines`      | The number of cache lines of the L2 cache                          | `256`              |
| `--l1-latency`    | The latency of the L1 cache in cycles                              | `4`                |
| `--l2-latency`    | The latency of the L2 cache in cycles                              | `12`               |
| `--memory-latency`| The latency of the main memory in cycles                           | `100`              |
| `--tf=<filepath>` | Output file for a trace file with all signals                      | `default_trace.vcd`|
| `--num-requests`  | Number of request to read from .csv file, default is all requests  | All requests       |
| `-h`, `--help`    | Display this help and exit                                         | N/A                |

## Theoretical Results
Document the results of the theoretical part of your project here. This could include findings from your research on cache sizes, architectures, and latencies in modern processors.

## Contributors
For more details, the `@author` or `@authors` is included in each Docstring.

### Alexander Anthony Tang
Alexander is primarily responsible for the development of `src/modules`, with additional contributions to the development of `src/main/simulator.cpp`

Other contributions include:
- Creating the `Makefile`
- Writing the `build.sh` script
- Initiating the project
- Documenting the project

### Lie Leon Alexius
Leon is primarily responsible for the development of `examples`, `src/main` and `src/assets`, with additional contributions to the development of `src/modules`

Other contributions include:
- Writing the `build.sh` and `setup.sh` scripts
- Authoring the `Readme.md`
- Initiating and structuring the project
- Optimizing the project
- Implementing automated tests
- Documenting the project

### Trang Nguyen
Trang is primarily responsible for the development of `src/modules`.

Other contributions include:
- Initiating the project
- Optimizing the project
- Documenting the project

## License
All Rights Reserved © 2024 Team-150.

This project and its contents are the property of Team-150. No reproduction, distribution, or transmission of the copyrighted materials in this project is permitted without the written permission of Team-150.
