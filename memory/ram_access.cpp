#include <iostream>
#include <fstream> // Include this for std::ofstream
#include <vector>
#include <chrono>
#include <cmath>
#include <cstring>

const int REPEAT_TIMES = 1000;
const int ACCESS_TIMES = 10000; 
const int ARRAY_SIZES[] = {4, 16, 64, 1024, 4096, 16384, 65536, 262144, 1048576, 4194304, 16777216, 67108864}; 
const int STRIDES[] = {4, 16, 64, 256, 1024, 4096}; 
void initializeArray(std::vector<int>& array, int stride) {
    int size = array.size();
    for (int i = 0; i < size; i++) {
        array[i] = (i + stride) % size;
    }
}

double measureLatency(int arraySize, int stride) {
    std::vector<int> array(arraySize);
    initializeArray(array, stride);

    int nextIndex = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ACCESS_TIMES; i++) {
        nextIndex = array[nextIndex];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> total_time = end - start;

    return total_time.count() / ACCESS_TIMES;
}

int main() {
    std::ofstream resultFile("memory_access_latency.csv");
    if (!resultFile) {
        std::cerr << "Failed to open result file." << std::endl;
        return 1;
    }

    resultFile << "Array Size (Bytes),Stride (Bytes),Latency (ns)\n";

    for (int arraySize : ARRAY_SIZES) {
        for (int stride : STRIDES) {
            double totalLatency = 0.0;

            for (int repeat = 0; repeat < REPEAT_TIMES; repeat++) {
                totalLatency += measureLatency(arraySize, stride);
            }

            double averageLatency = totalLatency / REPEAT_TIMES;

            std::cout << "Array Size: " << arraySize * sizeof(int)
                      << " Bytes, Stride: " << stride * sizeof(int)
                      << " Bytes, Latency: " << averageLatency << " ns" << std::endl;

            resultFile << arraySize * sizeof(int) << "," << stride * sizeof(int) << "," << averageLatency << "\n";
        }
    }

    resultFile.close();
    std::cout << "Results saved to memory_access_latency.csv" << std::endl;
    return 0;
}









