#include <iostream>
#include <windows.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "helper.h"

HANDLE event1, event2;

DWORD WINAPI contextSwitchTest(LPVOID param) {
    auto* switchTimes = static_cast<std::vector<double>*>(param);
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    for (int i = 0; i < switchTimes->size(); ++i) {
        WaitForSingleObject(event1, INFINITE);   
        QueryPerformanceCounter(&end);           
        QueryPerformanceCounter(&start);         
        SetEvent(event2);                        

        double elapsed_ns = (end.QuadPart - start.QuadPart) * 1e9 / frequency.QuadPart;
        (*switchTimes)[i] = elapsed_ns;
    }
    return 0;
}

int main() {
    const int iterations = 1000;
    event1 = CreateEvent(nullptr, FALSE, FALSE, "Event1");
    event2 = CreateEvent(nullptr, FALSE, FALSE, "Event2");

    if (!event1 || !event2) {
        return 1;
    }

    std::vector<double> threadSwitchTimes(iterations);
    std::vector<double> processSwitchTimes(iterations);

    // Measure thread context switch
    HANDLE thread = CreateThread(nullptr, 0, contextSwitchTest, &threadSwitchTimes, 0, nullptr);

    if (!thread) {
        return 1;
    }

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    std::ofstream rawFile("context_switch_raw_data.txt");
    if (!rawFile) {
        return 1;
    }

    for (int i = 0; i < iterations; ++i) {
        QueryPerformanceCounter(&start);
        SetEvent(event1);                        
        WaitForSingleObject(event2, INFINITE);   
        QueryPerformanceCounter(&end);

        double elapsed_ns = (end.QuadPart - start.QuadPart) * 1e9 / frequency.QuadPart;
        threadSwitchTimes[i] = elapsed_ns;
        rawFile << "Thread Context Switch Time [" << i + 1 << "]: " << elapsed_ns << " ns\n";
    }

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);

    // Measure process context switch
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    char command[] = "switching_process.exe";

    for (int i = 0; i < iterations; ++i) {
        if (CreateProcess(nullptr, command, nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
            QueryPerformanceCounter(&start);
            SetEvent(event1);                        
            WaitForSingleObject(event2, INFINITE);   
            QueryPerformanceCounter(&end);

            double elapsed_ns = (end.QuadPart - start.QuadPart) * 1e9 / frequency.QuadPart;
            processSwitchTimes[i] = elapsed_ns;
            rawFile << "Process Context Switch Time [" << i + 1 << "]: " << elapsed_ns << " ns\n";

            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    rawFile.close();
    CloseHandle(event1);
    CloseHandle(event2);

    double trimmedMeanThreadSwitchTime = calculateMean(threadSwitchTimes);
    double stdDevThreadSwitchTime = calculateStandardDeviation(threadSwitchTimes, trimmedMeanThreadSwitchTime);
    double stdDevPercentThreadSwitchTime = calculateStdDevPercentage(threadSwitchTimes, trimmedMeanThreadSwitchTime);

    std::cout << "Mean time: " << trimmedMeanThreadSwitchTime / 2 << " ns\n";
    std::cout << "Standard deviation: " << stdDevThreadSwitchTime << " ns\n";
    std::cout << "Standard deviation percent: " << stdDevPercentThreadSwitchTime << " %\n";

    double trimmedMeanProcessSwitchTime = calculateMean(processSwitchTimes;
    double stdDevProcessSwitchTime = calculateStandardDeviation(processSwitchTimes, trimmedMeanProcessSwitchTime);
    double stdDevPercentProcessSwitchTime = calculateStdDevPercentage(processSwitchTimes, trimmedMeanProcessSwitchTime);

    std::cout << "Mean time: " << trimmedMeanProcessSwitchTime / 2 << " ns\n";
    std::cout << "Standard deviation: " << stdDevProcessSwitchTime << " ns\n";
    std::cout << "Standard deviation percent: " << stdDevPercentProcessSwitchTime << " %\n";
    return 0;
}










