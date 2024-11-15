#include <chrono>
#include <iostream>
#include <vector>

void testFunction() {}
void testFunction(int a1) {}
void testFunction(int a1, int a2) {}
void testFunction(int a1, int a2, int a3) {}
void testFunction(int a1, int a2, int a3, int a4) {}
void testFunction(int a1, int a2, int a3, int a4, int a5) {}
void testFunction(int a1, int a2, int a3, int a4, int a5, int a6) {}
void testFunction(int a1, int a2, int a3, int a4, int a5, int a6, int a7) {}

template<typename Func>
double measureCallOverhead(Func func, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration = end - start;
    
    return duration.count() / iterations; 
}

int main() {
    const int iterations = 10000000;
    std::vector<double> overheads;

    overheads.push_back(measureCallOverhead([](){ testFunction(); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1, 2); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4, 5); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4, 5, 6); }, iterations));
    overheads.push_back(measureCallOverhead([](){ testFunction(1, 2, 3, 4, 5, 6, 7); }, iterations));

    std::cout << "Call Overhead (in nanoseconds):\n";
    for (int i = 0; i < overheads.size(); ++i) {
        std::cout << i << " arguments: " << overheads[i] << " ns\n";
    }

    std::cout << "\nIncremental Overhead per Argument:\n";
    for (int i = 1; i < overheads.size(); ++i) {
        double incremental = overheads[i] - overheads[i - 1];
        std::cout << i << " -> " << i + 1 << " arguments: " << incremental << " ns\n";
    }

    return 0;
}
