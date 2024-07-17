# Project Report: Simulation, Analysis, and Optimization of Memory Hierarchy Systems

## Introduction
This report documents the results of Project Group 150 on the simulation, analysis, and optimization of the L1 and L2 cache memory hierarchy systems in modern computer systems.


## Background
Caches is a strategy to mitigate the Von Neumann bottleneck.

Caches are small but fast memories that temporarily store data and instructions to reduce the access time to frequently used data. In modern processors, L1 (Level 1) and L2 (Level 2) caches are crucial. The L1 cache is the smallest and fastest memory, while the L2 cache is larger but somewhat slower.



## Methodology
1. **Simulation of the current cache architecture:**
    - Creation of a simulation model of the L1 and L2 cache structure.
    - Performance testing and analysis of the initial states (without optimization).
2. **Implementation of a command-line interface for inputting simulation parameters such as cache line size.**
3. **Implementation of optimization strategies:**
    - **Store-Back Buffer:** Introduction of a buffer that caches write operations to relieve the memory bus.
    - **Prefetch Buffer:** Prefetching frequently needed data with the custom buffer size
4. **Analysis and evaluation:**
    - Comparison of performance before and after the implementation of the optimizations.
    - Measurement of access times, hit rates, and overall system performance.

## Results
1. **Simulation of the memory hierarchy system architecture:**
    - The initial simulation showed average hit rates of 94% in the L1 cache and 87% in the L2 cache.
    - Bottlenecks in the simulation are primarily caused by waiting for fetching data from memory
3. **Implementation and impact of optimization strategies:**
    - **Store-Back Buffer:** Reduced average write access times by??
    - **Prefetch Buffer:** Increased hit rates in the L2 cache with our matrix multiplication algorithm from 23.8% to 40.5%  with a prefetch buffer with the size of 4 cache lines by preloading frequently needed data.
4. **Analysis and evaluation:**
    - The optimized caches with **Store-Back Buffer** showed improved efficiency in case of cycles
    - The optimized caches with **Prefetch Buffer** showed significantly improved efficiency in case of hit rates in L2


## Discussion
The implementation of the Store-Back Buffer and Prefetch Buffer showed the potential in improving cache performance, particularly the reduction in write access times through store back buffer and the increase in hit rates through prefetching data

## Contributors

### Alexander Anthony Tang
Alexander is primarily responsible for the development of `src/modules`, with additional contributions to the development of `src/main/simulator.cpp`

Other contributions include:
- Writing the `Makefile`
- Writing the `build.sh` script
- Initiating, Optimizing and Documenting the project
### Lie Leon Alexius
Leon is primarily responsible for the development of `examples`, `src/main` and `src/assets`, with additional contributions to the development of `src/modules`

Other contributions include:
- Writing the `Makefile`
- Writing the `build.sh` scripts
- Authoring the `Readme.md`
- Implementing automated tests
- Initiating, Optimizing, Structuring and Documenting the project

### Trang Nguyen
Trang is primarily responsible for the development of `src/modules`.

Other contributions include:
- Initiating, Optimizing and Documenting the project


## License
All Rights Reserved © 2024 Team-150.

This project and its contents are the property of Team-150. No reproduction, distribution, or transmission of the copyrighted materials in this project is permitted without the written permission of Team-150.
