#include <windows.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include "helper.h" 

void GenerateLargeFile(const std::string& filename, size_t sizeInMB) {
    const size_t sizeInBytes = sizeInMB * 1024 * 1024;
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return;
    }

    std::vector<char> buffer(1024 * 1024, 'A'); 
    for (size_t i = 0; i < sizeInMB; ++i) {
        file.write(buffer.data(), buffer.size());
    }

    file.close();
    std::cout << "File generated: " << filename << " (" << sizeInMB << " MB)\n";
}


void MeasureFaultedAccesses(const std::string& filename, size_t sizeInMB) {
    const size_t PAGE_SIZE = 4096;
    const size_t sizeInBytes = sizeInMB * 1024 * 1024;

    HANDLE hFile = CreateFile(
        filename.c_str(),
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ,              
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    HANDLE hMapping = CreateFileMapping(
        hFile,
        NULL,
        PAGE_READONLY,
        0,
        0,
        NULL
    );

    if (!hMapping) {
        std::cerr << "Failed to create file mapping!" << std::endl;
        CloseHandle(hFile);
        return;
    }

    char* buffer = (char*)MapViewOfFile(
        hMapping,
        FILE_MAP_READ,
        0,
        0,
        0
    );

    if (!buffer) {
        std::cerr << "Failed to map file view!" << std::endl;
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    std::vector<double> faultedAccessTimes;

    for (size_t i = 0; i < sizeInBytes; i += PAGE_SIZE) {
        auto start = std::chrono::high_resolution_clock::now();
        volatile char value = buffer[i]; // Access each page
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> elapsed = end - start;
        double timeTaken = elapsed.count();


        if (timeTaken > 0) {
            faultedAccessTimes.push_back(timeTaken);
        }
    }


    UnmapViewOfFile(buffer);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    if (!faultedAccessTimes.empty()) {
        double mean = calculateMean(faultedAccessTimes);
        double stdDev = calculateStandardDeviation(faultedAccessTimes, mean);

        std::cout << "Faulted accesses statistics:\n";
        std::cout << "Mean time: " << mean << " microseconds\n";
        std::cout << "Standard deviation: " << stdDev << " microseconds\n";
    } else {
        std::cout << "No faulted accesses were recorded.\n";
    }
}

int main() {
    const std::string filename = "large_test_file.bin";
    const size_t fileSizeMB = 1600; 

    GenerateLargeFile(filename, fileSizeMB);

    std::cout << "Measuring page fault times...\n";
    MeasureFaultedAccesses(filename, fileSizeMB);

    return 0;
}























