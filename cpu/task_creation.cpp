#include <iostream>
#include <windows.h>
#include <process.h>
#include <vector>
#include <fstream>
#include "helper.h"


unsigned long long rdtsc() {
    unsigned int lo, hi;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long)hi << 32) | lo;
}


double measureProcessCreation(int iterations, std::vector<double>& times, std::ofstream& outFile) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    for (int i = 0; i < iterations; ++i) {
        unsigned long long start = rdtsc();
        
        BOOL status = CreateProcess(nullptr, LPSTR("dummy.exe"), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);

        unsigned long long end = rdtsc();
        if (status) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

        double cycleCount = static_cast<double>(end - start);
        times.push_back(cycleCount);


        outFile << "Process Creation Cycle Count [" << i + 1 << "]: " << cycleCount << "\n";
    }

    double mean = calculateTrimmedMean(times, 0.05);  // Trim 5% from each end
    double stddevPercent = calculateStdDevPercentage(times, mean);
    std::cout << "Process creation overhead:\n";
    std::cout << "Mean (trimmed): " << mean << " cycles\n";
    std::cout << "Standard Deviation: " << stddevPercent << "% of the mean\n";
    return mean;
}


double measureThreadCreation(int iterations, std::vector<double>& times, std::ofstream& outFile) {
    for (int i = 0; i < iterations; ++i) {
        unsigned long long start = rdtsc();

        HANDLE thread = reinterpret_cast<HANDLE>(_beginthread([](void*) {}, 0, nullptr));

        unsigned long long end = rdtsc();
        if (thread) {
            CloseHandle(thread);
        }

        double cycleCount = static_cast<double>(end - start);
        times.push_back(cycleCount);


        outFile << "Thread Creation Cycle Count [" << i + 1 << "]: " << cycleCount << "\n";
    }

    double mean = calculateTrimmedMean(times, 0.05);  
    double stddevPercent = calculateStdDevPercentage(times, mean);
    std::cout << "Thread creation overhead:\n";
    std::cout << "Mean (trimmed): " << mean << " cycles\n";
    std::cout << "Standard Deviation: " << stddevPercent << "% of the mean\n";
    return mean;
}

int main() {
    const int iterations = 10000;
    std::vector<double> processCycles;
    std::vector<double> threadCycles;

    std::ofstream outFile("task_creation_timings.txt");
    if (!outFile) {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    double processMean = measureProcessCreation(iterations, processCycles, outFile);
    double threadMean = measureThreadCreation(iterations, threadCycles, outFile);

    double overheadRatio = processMean / threadMean;
    std::cout << "\nProcess creation is approximately " << overheadRatio << " times slower than thread creation.\n";

    outFile.close(); 
    return 0;
}






