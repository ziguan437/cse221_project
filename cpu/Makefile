
CXX = g++
CXXFLAGS = -std=c++11 -O0


INCLUDE_DIR = ../include
CPU_DIR = .


SOURCES = $(wildcard $(CPU_DIR)/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


EXECUTABLES = context_switch loop_measurement procedure_overhead syscall task_creation timer_measurement switching_process dummy


all: $(EXECUTABLES)

context_switch: context_switch.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

loop_measurement: loop_measurement.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

procedure_overhead: procedure_overhead.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

syscall: syscall.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

task_creation: task_creation.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

timer_measurement: timer_measurement.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< $(INCLUDE_DIR)/helper.cpp -o $@

dummy: dummy_process.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $< -o $@

clean:
	rm -f context_switch.exe loop_measurement.exe procedure_overhead.exe syscall.exe task_creation.exe timer_measurement.exe dummy.exe switching_process.exe
	rm -f loop_measurement.o procedure_overhead.o context_switch.o task_creation.o timer_measurement.o syscall.o dummy_process.o switching_process.o
	rm -f *.o *.txt


.PHONY: all clean
