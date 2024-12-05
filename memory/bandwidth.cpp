#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <windows.h>
#include "helper.h" 

const int TEST_SIZE_MB = 128; 
const int ITERATIONS = 50;
constexpr int MB_TO_INTS = 1024 * 1024 / sizeof(int);

void setHighPriority() {
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetProcessAffinityMask(GetCurrentProcess(), 1);
}

void flushCache(std::vector<int>& memory) {
    for (volatile int& x : memory) {
        x = 0;
    }
}

double measure_read_bandwidth(int* arr, int size_in_ints) {
    volatile int sink = 0; // Prevent optimization
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < size_in_ints; j += 32) {
            sink = arr[j];
            sink = arr[j + 1];
            sink = arr[j + 2];
            sink = arr[j + 3];
            sink = arr[j + 4];
            sink = arr[j + 5];
            sink = arr[j + 6];
            sink = arr[j + 7];
            sink = arr[j + 8];
            sink = arr[j + 9];
            sink = arr[j + 10];
            sink = arr[j + 11];
            sink = arr[j + 12];
            sink = arr[j + 13];
            sink = arr[j + 14];
            sink = arr[j + 15];
            sink = arr[j + 16];
            sink = arr[j + 17];
            sink = arr[j + 18];
            sink = arr[j + 19];
            sink = arr[j + 20];
            sink = arr[j + 21];
            sink = arr[j + 22];
            sink = arr[j + 23];
            sink = arr[j + 24];
            sink = arr[j + 25];
            sink = arr[j + 26];
            sink = arr[j + 27];
            sink = arr[j + 28];
            sink = arr[j + 29];
            sink = arr[j + 30];
            sink = arr[j + 31];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double bytes_read = static_cast<double>(size_in_ints * sizeof(int) * ITERATIONS);
    return (bytes_read / (1024 * 1024)) / elapsed.count(); 
}

double measure_write_bandwidth(int* arr, int size_in_ints) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < size_in_ints; j += 32) {
            arr[j] = i;
            arr[j + 1] = i;
            arr[j + 2] = i;
            arr[j + 3] = i;
            arr[j + 4] = i;
            arr[j + 5] = i;
            arr[j + 6] = i;
            arr[j + 7] = i;
            arr[j + 8] = i;
            arr[j + 9] = i;
            arr[j + 10] = i;
            arr[j + 11] = i;
            arr[j + 12] = i;
            arr[j + 13] = i;
            arr[j + 14] = i;
            arr[j + 15] = i;
            arr[j + 16] = i;
            arr[j + 17] = i;
            arr[j + 18] = i;
            arr[j + 19] = i;
            arr[j + 20] = i;
            arr[j + 21] = i;
            arr[j + 22] = i;
            arr[j + 23] = i;
            arr[j + 24] = i;
            arr[j + 25] = i;
            arr[j + 26] = i;
            arr[j + 27] = i;
            arr[j + 28] = i;
            arr[j + 29] = i;
            arr[j + 30] = i;
            arr[j + 31] = i;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double bytes_written = static_cast<double>(size_in_ints * sizeof(int) * ITERATIONS);
    return (bytes_written / (1024 * 1024)) / elapsed.count();  
}


int main() {
    setHighPriority(); 

    const int size_in_ints = TEST_SIZE_MB * MB_TO_INTS;
    std::vector<int> memory(size_in_ints, 0); 

    flushCache(memory);

    std::cout << "Memory Size (MB), Read Bandwidth (MB/s), Write Bandwidth (MB/s), Read Std Dev (MB/s), Write Std Dev (MB/s)\n";

    std::vector<double> read_bandwidths;
    std::vector<double> write_bandwidths;

    for (int i = 0; i < 5; ++i) {  
        double read_bw = measure_read_bandwidth(memory.data(), size_in_ints);
        double write_bw = measure_write_bandwidth(memory.data(), size_in_ints);
        read_bandwidths.push_back(read_bw);
        write_bandwidths.push_back(write_bw);
    }

    double mean_read = calculateMean(read_bandwidths);
    double mean_write = calculateMean(write_bandwidths);
    double std_dev_read = calculateStandardDeviation(read_bandwidths, mean_read);
    double std_dev_write = calculateStandardDeviation(write_bandwidths, mean_write);

    std::cout << TEST_SIZE_MB << ", "
              << mean_read << ", " << mean_write << ", "
              << std_dev_read << ", " << std_dev_write << "\n";

    return 0;
}


