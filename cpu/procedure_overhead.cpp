#include <chrono>
#include <iostream>
#include <vector>
#include "helper.h" 

void testFunction() {}
void testFunction(int a1) {}
void testFunction(int a1, int a2) {}
void testFunction(int a1, int a2, int a3) {}
void testFunction(int a1, int a2, int a3, int a4) {}
void testFunction(int a1, int a2, int a3, int a4, int a5) {}
void testFunction(int a1, int a2, int a3, int a4, int a5, int a6) {}
void testFunction(int a1, int a2, int a3, int a4, int a5, int a6, int a7) {}

template<typename Func>
std::vector<double> measureCallOverhead(Func func, int iterations, int num_runs) {
    std::vector<double> overheads;
    for (int run = 0; run < num_runs; ++run) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> duration = end - start;
        overheads.push_back(duration.count() / iterations);
    }
    return overheads;
}

int main() {
    const int iterations = 10000000;
    const int num_runs = 10; 

    std::vector<std::vector<double>> all_overheads;

    // Measure overhead for each number of arguments
    all_overheads.push_back(measureCallOverhead([](){ testFunction(); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1, 2); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4, 5); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4, 5, 6); }, iterations, num_runs));
    all_overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4, 5, 6, 7); }, iterations, num_runs));

    // Print results
    for (size_t i = 0; i < all_overheads.size(); ++i) {
        double mean = calculateMean(all_overheads[i]);
        double stddev = calculateStandardDeviation(all_overheads[i], mean);
        std::cout << i << " arguments: Mean = " << mean 
                  << " ns, StdDev = " << stddev << " ns\n";
    }

    for (size_t i = 1; i < all_overheads.size(); ++i) {
        double mean1 = calculateMean(all_overheads[i - 1]);
        double mean2 = calculateMean(all_overheads[i]);
        double incremental = mean2 - mean1;
        std::cout << i << " -> " << i + 1 << " arguments: " << incremental << " ns\n";
    }

    return 0;
}

