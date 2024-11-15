#include <chrono>
#include <iostream>
#include <vector>
#include <fstream>  
#include "helper.h"  

void timer_measurement() {
    const int num_runs = 1000;
    std::vector<double> timings;
    timings.reserve(num_runs); 

    for (int j = 0; j < num_runs; ++j) {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < 100000; ++i) {
            std::chrono::high_resolution_clock::now();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        timings.push_back(static_cast<double>(elapsed.count()));
    }


    std::ofstream outFile("timer_measurements.txt");
    if (!outFile) {
        std::cerr << "Failed to open output file.\n";
        return;
    }

    for (size_t i = 0; i < timings.size(); ++i) {
        outFile << "Measurement [" << i + 1 << "]: " << timings[i] << " microseconds\n";
    }


    outFile.close();


    double trimmed_mean_time = calculateTrimmedMean(timings, 0.05);
    std::cout << "Trimmed mean time over " << num_runs << " runs: " << trimmed_mean_time << " microseconds\n";


    double stddev = calculateStandardDeviation(timings, trimmed_mean_time);
    std::cout << "Standard deviation: " << stddev << " microseconds\n";

    double stddev_percent = calculateStdDevPercentage(timings, trimmed_mean_time);
    std::cout << "Standard deviation percentage: " << stddev_percent << "% of the mean\n";
}

int main() {
    timer_measurement();
    return 0;
}



