#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include "helper.h"

const int MAX_SIZE_MB = 40;  
const int STEP_SIZE_MB = 1;   
const int ITERATIONS = 50;    


constexpr int MB_TO_INTS = 1024 * 1024 / sizeof(int);

double measure_read_bandwidth(int* arr, int size_in_ints) {
    volatile int sink = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < size_in_ints; j += 8) {
            sink += arr[j];
            sink += arr[j + 1];
            sink += arr[j + 2];
            sink += arr[j + 3];
            sink += arr[j + 4];
            sink += arr[j + 5];
            sink += arr[j + 6];
            sink += arr[j + 7];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double bytes_read = static_cast<double>(size_in_ints * sizeof(int) * ITERATIONS);
    return (bytes_read / (1024 * 1024)) / elapsed.count();  // Return bandwidth in MB/s
}


double measure_write_bandwidth(int* arr, int size_in_ints) {
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++) {
        for (int j = 0; j < size_in_ints; j += 8) {
            arr[j] = i;
            arr[j + 1] = i;
            arr[j + 2] = i;
            arr[j + 3] = i;
            arr[j + 4] = i;
            arr[j + 5] = i;
            arr[j + 6] = i;
            arr[j + 7] = i;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double bytes_written = static_cast<double>(size_in_ints * sizeof(int) * ITERATIONS);
    return (bytes_written / (1024 * 1024)) / elapsed.count();  
}

int main() {
    std::cout << "Memory Size (MB), Read Bandwidth (MB/s), Write Bandwidth (MB/s), Read Std Dev %, Write Std Dev %\n";


    std::vector<int> memory(MAX_SIZE_MB * MB_TO_INTS, 0);
    std::vector<double> all_read_bandwidths;
    std::vector<double> all_write_bandwidths;

    for (int size_mb = STEP_SIZE_MB; size_mb <= MAX_SIZE_MB; size_mb += STEP_SIZE_MB) {
        int size_in_ints = size_mb * MB_TO_INTS;
        
        std::vector<double> read_bandwidths;
        std::vector<double> write_bandwidths;


        for (int i = 0; i < 5; ++i) {  
            double read_bw = measure_read_bandwidth(memory.data(), size_in_ints);
            double write_bw = measure_write_bandwidth(memory.data(), size_in_ints);
            read_bandwidths.push_back(read_bw);
            write_bandwidths.push_back(write_bw);
            all_read_bandwidths.push_back(read_bw);
            all_write_bandwidths.push_back(write_bw);
        }

        
        double mean_read = calculateMean(read_bandwidths);
        double mean_write = calculateMean(write_bandwidths);
        double std_dev_read = calculateStandardDeviation(read_bandwidths, mean_read);
        double std_dev_write = calculateStandardDeviation(write_bandwidths, mean_write);
        double std_dev_percent_read = calculateStdDevPercentage(read_bandwidths, mean_read);
        double std_dev_percent_write = calculateStdDevPercentage(write_bandwidths, mean_write);

        std::cout << size_mb << ", " 
                  << mean_read << ", " << mean_write << ", "
                  << std_dev_percent_read << ", " << std_dev_percent_write << "\n";
    }

    // Calculate and output overall mean and standard deviation percentage
    double overall_mean_read = calculateMean(all_read_bandwidths);
    double overall_mean_write = calculateMean(all_write_bandwidths);
    double overall_std_dev_percent_read = calculateStdDevPercentage(all_read_bandwidths, overall_mean_read);
    double overall_std_dev_percent_write = calculateStdDevPercentage(all_write_bandwidths, overall_mean_write);

    std::cout << "\nOverall Read Bandwidth Mean: " << overall_mean_read << " MB/s\n";
    std::cout << "Overall Read Bandwidth Std Dev %: " << overall_std_dev_percent_read << " %\n";
    std::cout << "Overall Write Bandwidth Mean: " << overall_mean_write << " MB/s\n";
    std::cout << "Overall Write Bandwidth Std Dev %: " << overall_std_dev_percent_write << " %\n";

    return 0;
}


