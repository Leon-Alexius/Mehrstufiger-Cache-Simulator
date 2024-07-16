# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# Entry point for the program and target name
# Determine variables that holds the paths to the source files that need to be compiled
C_SRCS = src/main/executor.c 
PARSER = src/main/parser/csv_parser.c src/main/parser/parse.c src/main/parser/terminal_parser.c
GRAPHER = src/main/grapher/printer.c
CPP_SRCS = src/main/simulator.cpp
ANALYZE_C_SRCS = src/assets/analysis/matrix_analyze.c
ANALYZE_CPP_SRCS = src/assets/analysis/simulator_analyze.cpp
MATRIX_C_SRCS = examples/matrix.c
MATRIX_CSV_SRCS = examples/matrix_csv.c

# Object files
# variables to the paths of the object files that will be generated from the C and C++ source files
# Pattern substitution: replace the .c and .cpp extensions with .o
C_OBJS = $(C_SRCS:.c=.o) $(PARSER:.c=.o) $(GRAPHER:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

ANALYZE_C_OBJS = $(ANALYZE_C_SRCS:.c=.o) $(PARSER:.c=.o) $(GRAPHER:.c=.o)
ANALYZE_CPP_OBJS = $(ANALYZE_CPP_SRCS:.cpp=.o)

MATRIX_C_OBJS = $(MATRIX_C_SRCS:.c=.o)
MATRIX_CSV_OBJS = $(MATRIX_CSV_SRCS:.c=.o)

# Variable to the paths of the header files that the source files depend on
HEADERS := src/modules/module.hpp

# Target name
# The name of the final executable that will be generated
TARGET := cache
ANALYZE_TARGET := src/assets/analysis/analyze
CSV_TARGET := examples/matrix_csv
MATRIX_TARGET := examples/matrix

# The path to SystemC installation (this project included Systemc to standardize the path)
SCPATH = systemc

# Flags for the C++ compiler
# 1. C++ standard 14 (-std=c++14)
# 2. paths to include files (-I$(SCPATH)/include) 
# 3. paths to library files (-L$(SCPATH)/lib)
# 4. libraries to link against (-lsystemc -lm)
# 5. Warnings (-W -Wall -Wextra)
CXXFLAGS := -std=c++14  -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm

# Flags for the C compiler
# 1. Address Sanitizer
# CFLAGS := -fsanitize=address

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# Determine if clang or gcc is available
# CXX run the C++ compiler
CXX := $(shell command -v g++ || command -v clang++)
ifeq ($(strip $(CXX)),)
    $(error Neither clang++ nor g++ is available. Exiting.)
endif

# CC run the C compiler
CC := $(shell command -v gcc || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Neither clang nor g is available. Exiting.)
endif

# Add flag to CXXFLAGS if not MacOS
UNAME_S := $(shell uname -s) #  Get the name of the operating system currently in use
ifneq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath=$(SCPATH)/lib
endif

# Pattern rules for make: build .o files from .c and .cpp files
# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Targets in Makefile
.PHONY: all debug release clean analyze

# Default to release build for both app and library
all: debug

# Debug build
debug: CXXFLAGS += -g  # include debugging information in the output file -fsanitize=address
debug: $(TARGET)
debug: 
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o 

# Release build
release: CXXFLAGS += -O2 # optimize the code using O2
release: $(TARGET)
release:
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o 

# Rule to link object files to executables, flags, etc.
$(TARGET): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(CFLAGS) $(C_OBJS) $(CPP_OBJS) $(LDFLAGS) -o $(TARGET)

$(ANALYZE_TARGET): $(ANALYZE_C_OBJS) $(ANALYZE_CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(ANALYZE_C_OBJS) $(ANALYZE_CPP_OBJS) $(LDFLAGS) -o $(ANALYZE_TARGET)

$(CSV_TARGET): $(MATRIX_CSV_OBJS)
	$(CXX) $(MATRIX_CSV_OBJS) $(LDFLAGS) -o $(CSV_TARGET)

$(MATRIX_TARGET): $(MATRIX_C_OBJS)
	$(CXX) $(MATRIX_C_OBJS) $(LDFLAGS) -o $(MATRIX_TARGET)

analyze: CXXFLAGS += -O2
analyze: $(ANALYZE_TARGET)
analyze:
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o 
	rm -rf src/assets/analysis/*.o

matrix: CXXFLAGS += -O2
matrix: $(MATRIX_TARGET)
matrix: 
	rm -rf examples/*.o 

csv: CXXFLAGS += -O2
csv: $(CSV_TARGET)
csv: 
	rm -rf examples/*.o 

# clean up
clean:
	rm -f $(TARGET)
	rm -f $(ANALYZE_TARGET)
	rm -f $(MATRIX_TARGET)
	rm -f $(CSV_TARGET)
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o
	rm -rf src/assets/analysis/*.o
	rm -rf examples/*.o
