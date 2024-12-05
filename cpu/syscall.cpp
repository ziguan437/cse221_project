#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <numeric>
#include <windows.h>
#include "helper.h"  

// Minimal function to measure procedure call overhead
void emptyFunction() {
}

template<typename Func>
void measureBatchOverhead(Func func, int res, int iterations, std::vector<double>& times) {

    for (int j = 0; j < res; j++ ) {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; ++i) {
            func();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;

        times.push_back(elapsed.count() / iterations); 
    }

}

int main() {
    const int iterations = 10000000;  
    const int res = 10;
    std::vector<double> procedureTimes;
    std::vector<double> systemCallTimes;


    measureBatchOverhead([]() { emptyFunction(); }, res, iterations, procedureTimes);
    double procedureMean = calculateMean(procedureTimes);
    double procedureStdDevPercent = calculateStandardDeviation(procedureTimes, procedureMean);

    std::cout << "Procedure call overhead:\n";
    std::cout << "Mean: " << procedureMean << " ns\n";
    std::cout << "Standard Deviation: " << procedureStdDevPercent << "\n";


    measureBatchOverhead([]() { GetCurrentProcessId(); }, res, iterations, systemCallTimes);
    double systemCallMean = calculateMean(systemCallTimes);
    double systemCallStdDevPercent = calculateStandardDeviation(systemCallTimes, systemCallMean);

    std::cout << "\nSystem call (GetCurrentProcessId) overhead:\n";
    std::cout << "Mean: " << systemCallMean << " ns\n";
    std::cout << "Standard Deviation: " << systemCallStdDevPercent << "";
    return 0;
}







