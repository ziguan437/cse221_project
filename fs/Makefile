CXX = g++
CXXFLAGS = -std=c++17 -O0

INCLUDE_DIR = ../include
CPU_DIR = .

SOURCES = $(wildcard $(CPU_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLES = file_cache read_time remote_access contention

all: $(EXECUTABLES)

file_cache: file_cache.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

read_time: read_time.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

remote_access: remote_access.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@ -lmpr

contention: contention.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

clean:
	rm -f file_cache.exe read_time.exe remote_access.exe contention.exe
	rm -f file_cache.o read_time.o remote_access.o contention.o
	rm -f *.o *.txt

.PHONY: all clean
