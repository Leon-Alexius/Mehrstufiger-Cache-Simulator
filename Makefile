# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# Entry point for the program and target name
# Determine variables that holds the paths to the source files that need to be compiled
C_SRCS = src/main/executor.c 
PARSER = src/main/parser/csv_parser.c src/main/parser/parse.c src/main/parser/terminal_parser.c
GRAPHER = src/main/grapher/printer.c
CPP_SRCS = src/main/simulator.cpp

# Object files
# variables to the paths of the object files that will be generated from the C and C++ source files
# Pattern substitution: replace the .c and .cpp extensions with .o
C_OBJS = $(C_SRCS:.c=.o) $(PARSER:.c=.o) $(GRAPHER:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

# Target name
# The name of the final executable that will be generated
TARGET := cache

# The path to SystemC installation (this project included Systemc to standardize the path)
SCPATH = systemc

# Flags for the C++ compiler
# 1. C++ standard 14 (-std=c++14)
# 2. paths to include files (-I$(SCPATH)/include) 
# 3. paths to library files (-L$(SCPATH)/lib)
# 4. libraries to link against (-lsystemc -lm)
CXXFLAGS := -std=c++14  -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm

# Flags for the C compiler
# 1. C17 standard (-std=c17)
CFLAGS := -std=c17

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
.PHONY: all debug release clean

# Default to release build for both app and library
all: release

# Debug build
debug: CXXFLAGS += -g -fsanitize=address # include debugging information in the output file
debug: $(TARGET)
debug: 
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o 

# Release build
release: CXXFLAGS += -O3 # optimize the code using O3
release: CFLAGS += -O3 # optimize the code using O3
release: $(TARGET)
release:
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o 

# Rule to link object files to executables, flags, etc.
$(TARGET): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(CFLAGS) $(C_OBJS) $(CPP_OBJS) -o $(TARGET)

# clean up
clean:
	rm -f $(TARGET)
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o
