#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>


const int MAX_SIZE_MB = 128; 
const int STEP_SIZE_MB = 1;   
const int ITERATIONS = 1000000;  


constexpr int MB_TO_INTS = 1024 * 1024 / sizeof(int);


double measure_latency(int* arr, int size_in_ints, int stride) {
    volatile int sink = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; i++) {
        sink += arr[(i * stride) % size_in_ints];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed = end - start;
    return elapsed.count() / ITERATIONS;
}

int main() {
    std::cout << "Memory Size (MB), Latency (ns)\n";


    std::vector<int> memory(MAX_SIZE_MB * MB_TO_INTS, 1);

    for (int size_mb = STEP_SIZE_MB; size_mb <= MAX_SIZE_MB; size_mb += STEP_SIZE_MB) {
        int size_in_ints = size_mb * MB_TO_INTS;
        

        double latency_ns = measure_latency(memory.data(), size_in_ints, 64); 


        std::cout << size_mb << ", " << latency_ns << "\n";
    }

    return 0;
}






