#include <iostream>
#include <vector>
#include <cmath>       
#include <numeric>     
#include <windows.h>
#include <unistd.h>
#include "helper.h" 

// Minimal function to measure procedure call overhead with a more complex operation
void emptyFunction() {
}

// Alternative to __rdtsc() for GCC/Clang using inline assembly
unsigned long long rdtsc() {
    unsigned int lo, hi;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)hi << 32) | lo;
}


template<typename Func>
void measureBatchOverhead(Func func, int iterations, std::vector<double>& cycles) {
    unsigned long long start = rdtsc();  

    for (int i = 0; i < iterations; ++i) {
        func();
    }

    unsigned long long end = rdtsc();  

    cycles.push_back(static_cast<double>(end - start) / iterations);
}

int main() {
    const int iterations = 10000000;  
    std::vector<double> procedureCycles;
    std::vector<double> systemCallCycles;

    measureBatchOverhead([](){ emptyFunction(); }, iterations, procedureCycles);
    double procedureMean = calculateMean(procedureCycles);
    double procedureStdDevPercent = calculateStdDevPercentage(procedureCycles, procedureMean);

    std::cout << "Procedure call overhead:\n";
    std::cout << "Mean: " << procedureMean << " cycles\n";
    std::cout << "Standard Deviation: " << procedureStdDevPercent << "% of the mean\n";


    measureBatchOverhead([](){         
        volatile int pid = getpid();  
        (void)pid;  }, iterations, systemCallCycles);
    double systemCallMean = calculateMean(systemCallCycles);
    double systemCallStdDevPercent = calculateStdDevPercentage(systemCallCycles, systemCallMean);

    std::cout << "\nSystem call overhead:\n";
    std::cout << "Mean: " << systemCallMean << " cycles\n";
    std::cout << "Standard Deviation: " << systemCallStdDevPercent << "% of the mean\n";

    double overheadRatio = systemCallMean / procedureMean;
    std::cout << "\nSystem call is approximately " << overheadRatio << " times slower than a procedure call.\n";

    return 0;
}






