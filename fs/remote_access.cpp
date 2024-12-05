#include <windows.h>
#include <winnetwk.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <cmath>
#include <memory>
#include <algorithm>

void connect_to_remote(const std::string& remote_path, const std::string& username, const std::string& password) {
    NETRESOURCE nr = {0};
    nr.dwType = RESOURCETYPE_DISK;
    nr.lpLocalName = nullptr; 
    nr.lpRemoteName = const_cast<char*>(remote_path.c_str());
    nr.lpProvider = nullptr;

    DWORD result = WNetAddConnection2A(&nr, password.c_str(), username.c_str(), 0);
    if (result != NO_ERROR) {
        throw std::runtime_error("Failed to connect to remote share. Error code: " + std::to_string(result));
    }
}

void generate_file(const std::string& filename, size_t size_in_bytes) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to create file: " + filename);
    }
    std::vector<char> buffer(1024 * 1024, 'A'); 
    size_t bytes_written = 0;
    while (bytes_written < size_in_bytes) {
        size_t write_size = std::min(buffer.size(), size_in_bytes - bytes_written);
        file.write(buffer.data(), write_size);
        bytes_written += write_size;
    }
    file.close();
}

double measure_read_time(const std::string& filename, size_t block_size, bool random_access, int iterations = 5) {
    HANDLE file = CreateFileA(
        filename.c_str(),
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN,
        nullptr);

    if (file == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    size_t aligned_buffer_size = ((block_size + 4095) / 4096) * 4096; 
    auto aligned_buffer = std::make_unique<char[]>(aligned_buffer_size);

    LARGE_INTEGER file_size;
    GetFileSizeEx(file, &file_size);
    size_t num_blocks = static_cast<size_t>(file_size.QuadPart / block_size);

    std::vector<size_t> offsets(num_blocks);
    for (size_t i = 0; i < num_blocks; ++i) {
        offsets[i] = i * block_size;
    }

    if (random_access) {
        std::mt19937 rng(std::random_device{}());
        std::shuffle(offsets.begin(), offsets.end(), rng);
    }

    std::vector<double> iteration_times;

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();

        for (size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
            LARGE_INTEGER offset;
            offset.QuadPart = offsets[block_idx];

            if (random_access) {
                SetFilePointerEx(file, offset, nullptr, FILE_BEGIN);
            }

            DWORD bytes_read;
            ReadFile(file, aligned_buffer.get(), static_cast<DWORD>(block_size), &bytes_read, nullptr);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        iteration_times.push_back(elapsed.count() / num_blocks); 
    }

    CloseHandle(file);

    double total = 0;
    for (double time : iteration_times) {
        total += time;
    }
    return total / iteration_times.size();
}

int main() {
    try {
        const std::string remote_path = R"(My_path)"; 
        const std::string username = "NA"; // Replaced for privacy reason
        const std::string password = "NA"; // Replaced for privacy reason
        const std::string filename = remote_path + R"(\test_file.bin)";
        const size_t min_file_size = 4 * 1024;  
        const size_t max_file_size = 4 * 1024 * 1024; 
        const size_t step_factor = 2;  
        const size_t block_size = 4 * 1024;  
        const int iterations = 5;


        connect_to_remote(remote_path, username, password);

        std::ofstream results("results.csv");
        if (!results.is_open()) {
            throw std::runtime_error("Failed to create results file.");
        }
        results << "File Size (KB),Sequential Mean (s),Random Mean (s)\n";

        for (size_t size = min_file_size; size <= max_file_size; size *= step_factor) {
            generate_file(filename, size);

            double seq_mean = measure_read_time(filename, block_size, false, iterations);
            double rand_mean = measure_read_time(filename, block_size, true, iterations);

            std::cout << "File Size: " << (size / 1024) << " KB, Seq Mean: " << seq_mean
                      << " s, Rand Mean: " << rand_mean << " s\n";

            results << (size / 1024) << "," << seq_mean << "," << rand_mean << "\n";
        }

        results.close();
        DeleteFileA(filename.c_str());

        std::cout << "Results saved to results.csv\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
