# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O0

# Directories
INCLUDE_DIR = ../include
CPU_DIR = .

# Source files
SOURCES = $(wildcard $(CPU_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Executables to create
EXECUTABLES = ram_access bandwidth page_fault

# Compile each source file into an executable
all: $(EXECUTABLES)

ram_access: ram_access.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< -o $@

bandwidth: bandwidth.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

page_fault: page_fault.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

# Clean rule to remove all generated files
clean:
	-del ram_access.exe bandwidth.exe page_fault.exe
	-del ram_access.o bandwidth.o page_fault.o
	-del *.o *.txt


.PHONY: all clean