#include <chrono>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include "helper.h"


void measureLoopOverhead(int iterations, int runs) {
    std::vector<double> times;
    for (int j = 0; j < runs; ++j) {
        volatile int dummy = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            dummy += i;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        times.push_back(elapsed.count());
    }

    double mean = calculateMean(times);
    double stddev_percentage = calculateStdDevPercentage(times, mean);
    std::cout << "Loop overhead - Mean: " << mean << " microseconds, StdDev: " << stddev_percentage << "%\n";
}


void measureLoopUnrolling(int iterations, int runs) {
    std::vector<double> times_regular, times_unrolled;
    
    for (int j = 0; j < runs; ++j) {
        volatile int dummy = 0;
        

        auto start_regular = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            dummy += i;
        }
        auto end_regular = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed_regular = std::chrono::duration_cast<std::chrono::microseconds>(end_regular - start_regular);
        times_regular.push_back(elapsed_regular.count());
        

        auto start_unrolled = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; i += 4) {
            dummy += i;
            dummy += i + 1;
            dummy += i + 2;
            dummy += i + 3;
        }
        auto end_unrolled = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed_unrolled = std::chrono::duration_cast<std::chrono::microseconds>(end_unrolled - start_unrolled);
        times_unrolled.push_back(elapsed_unrolled.count());
    }

    double mean_regular = calculateMean(times_regular);
    double stddev_percentage_regular = calculateStdDevPercentage(times_regular, mean_regular);
    double mean_unrolled = calculateMean(times_unrolled);
    double stddev_percentage_unrolled = calculateStdDevPercentage(times_unrolled, mean_unrolled);

    std::cout << "Regular loop Mean: " << mean_regular << " microseconds, StdDev: " << stddev_percentage_regular << "%\n";
    std::cout << "Unrolled loop Mean: " << mean_unrolled << " microseconds, StdDev: " << stddev_percentage_unrolled << "%\n";
}


void measureBranchPrediction(int iterations, int runs) {
    std::vector<double> times_predictable, times_unpredictable;
    
    for (int j = 0; j < runs; ++j) {
        volatile int dummy = 0;
        
        auto start_predictable = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            if (i % 2 == 0) {
                dummy += i;
            } else {
                dummy -= i;
            }
        }
        auto end_predictable = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed_predictable = std::chrono::duration_cast<std::chrono::microseconds>(end_predictable - start_predictable);
        times_predictable.push_back(elapsed_predictable.count());
        
        auto start_unpredictable = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            if (rand() % 2 == 0) {
                dummy += i;
            } else {
                dummy -= i;
            }
        }
        auto end_unpredictable = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed_unpredictable = std::chrono::duration_cast<std::chrono::microseconds>(end_unpredictable - start_unpredictable);
        times_unpredictable.push_back(elapsed_unpredictable.count());
    }

    double mean_predictable = calculateMean(times_predictable);
    double stddev_percentage_predictable = calculateStdDevPercentage(times_predictable, mean_predictable);
    double mean_unpredictable = calculateMean(times_unpredictable);
    double stddev_percentage_unpredictable = calculateStdDevPercentage(times_unpredictable, mean_unpredictable);

    std::cout << "Predictable branch Mean: " << mean_predictable << " microseconds, StdDev: " << stddev_percentage_predictable << "%\n";
    std::cout << "Unpredictable branch Mean: " << mean_unpredictable << " microseconds, StdDev: " << stddev_percentage_unpredictable << "%\n";
}

int main() {
    const int iterations = 20000000;  // Number of loop iterations
    const int runs = 100;            // Number of test runs for averaging

    std::cout << "Measurement over 20000000 loops: \n";
    measureLoopOverhead(iterations, runs);
    measureLoopUnrolling(iterations, runs);
    measureBranchPrediction(iterations, runs);

    return 0;
}

