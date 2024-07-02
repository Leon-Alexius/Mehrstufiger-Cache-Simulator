# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# entry point for the program and target name
C_SRCS = src/main/executor.c 
PARSER = src/main/parser/csv_parser.c src/main/parser/parse.c src/main/parser/terminal_parser.c
GRAPHER = src/main/grapher/printer.c
CPP_SRCS = src/main/simulator.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o) $(PARSER:.c=.o) $(GRAPHER:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

# assignment task file
HEADERS := src/main/placeHolder.hpp

# target name
TARGET := cache

# Path to your systemc installation
SCPATH = systemc

# Additional flags for the compiler
CXXFLAGS := -std=c++14  -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm


# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# Determine if clang or gcc is available
CXX := $(shell command -v g++ || command -v clang++)
ifeq ($(strip $(CXX)),)
    $(error Neither clang++ nor g++ is available. Exiting.)
endif

CC := $(shell command -v gcc || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Neither clang nor g is available. Exiting.)
endif

# Add rpath except for MacOS
UNAME_S := $(shell uname -s)

ifneq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath=$(SCPATH)/lib
endif


# Default to release build for both app and library
all: debug

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS += -g
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O2
release: $(TARGET)

# Rule to link object files to executable
$(TARGET): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(C_OBJS) $(CPP_OBJS) $(LDFLAGS) -o $(TARGET)

# clean up
clean:
	rm -f $(TARGET)
	rm -rf src/main/parser/*.o 
	rm -rf src/main/grapher/*.o
	rm -rf src/main/*.o

.PHONY: all debug release clean