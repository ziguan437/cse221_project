#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <windows.h>
#include <fstream>
#include "helper.h"  

const int PAGE_SIZE = 4096;  
const int ITERATIONS = 500;

void measure_page_fault_time(std::vector<double>& times, std::ofstream& dataFile) {
    char buffer[PAGE_SIZE] = {};  // Buffer to fill the file

    for (int i = 0; i < ITERATIONS; ++i) {
        std::string filename = "page_fault_test_file_" + std::to_string(i) + ".bin";

        // Write a single page to a unique file
        {
            std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
            ofs.write(buffer, PAGE_SIZE);
            ofs.flush();
            ofs.close();
        }

        HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        HANDLE mapping = CreateFileMappingA(file, nullptr, PAGE_READONLY, 0, PAGE_SIZE, nullptr);
        if (mapping == nullptr) {
            CloseHandle(file);
            std::cerr << "Failed to create file mapping.\n";
            return;
        }

        // Map the file view
        char* mapped_view = static_cast<char*>(MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, PAGE_SIZE));
        if (mapped_view == nullptr) {
            CloseHandle(mapping);
            CloseHandle(file);
            std::cerr << "Failed to map file view.\n";
            return;
        }

        auto start = std::chrono::high_resolution_clock::now();
        volatile char first_byte = mapped_view[0];  
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::nano> elapsed = end - start;
        double elapsed_ns = elapsed.count();
        times.push_back(elapsed_ns);

        dataFile << "Iteration " << i + 1 << ": " << elapsed_ns << " ns\n";

        UnmapViewOfFile(mapped_view);
        CloseHandle(mapping);
        CloseHandle(file);


        std::remove(filename.c_str());
    }
}

int main() {
    std::vector<double> times;
    times.reserve(ITERATIONS);

    std::ofstream dataFile("page_fault_times.txt");
    if (!dataFile) {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    measure_page_fault_time(times, dataFile);
    dataFile.close();

    double mean_time = calculateMean(times);
    double stddev_time = calculateStandardDeviation(times, mean_time);
    double stddev_percent = calculateStdDevPercentage(times, mean_time);

    std::cout << "Average page fault time: " << mean_time << " ns\n";
    std::cout << "Standard deviation: " << stddev_time << " ns\n";
    std::cout << "Standard deviation percent: " << stddev_percent << "%\n";

    return 0;
}








