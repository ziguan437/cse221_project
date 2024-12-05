#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <windows.h>

void create_directory(const std::string& directory) {
    if (!CreateDirectoryA(directory.c_str(), NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            throw std::runtime_error("Failed to create directory: " + directory);
        }
    }
}

void remove_file(const std::string& filepath) {
    if (!DeleteFileA(filepath.c_str())) {
        throw std::runtime_error("Failed to delete file: " + filepath);
    }
}

void remove_directory(const std::string& directory) {
    if (!RemoveDirectoryA(directory.c_str())) {
        throw std::runtime_error("Failed to remove directory: " + directory);
    }
}

void generate_file(const std::string& filename, size_t size_in_bytes) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to create file: " + filename);
    }

    const size_t buffer_size = 1024 * 1024; 
    std::vector<char> buffer(buffer_size, 'A');

    size_t bytes_written = 0;
    while (bytes_written < size_in_bytes) {
        size_t chunk_size = std::min(buffer_size, size_in_bytes - bytes_written);
        file.write(buffer.data(), chunk_size);
        bytes_written += chunk_size;
    }

    file.close();
}


double measure_block_access_time_reverse(const std::string& filename, size_t block_size, int iterations = 5) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate); 
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    size_t file_size = static_cast<size_t>(file.tellg()); 
    if (file_size < block_size) {
        throw std::runtime_error("Block size is larger than the file size.");
    }

    std::vector<char> buffer(block_size);

    double total_time = 0.0;
    size_t total_blocks = 0;

    for (int i = 0; i < iterations; ++i) {
        size_t remaining_size = file_size;

        while (remaining_size > 0) {
            size_t current_block_size = std::min(block_size, remaining_size);
            size_t offset = remaining_size - current_block_size;

            auto start = std::chrono::high_resolution_clock::now();

            file.seekg(offset, std::ios::beg); 
            file.read(buffer.data(), current_block_size);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            if (file.gcount() > 0) {
                total_time += elapsed.count();
                total_blocks++;
            }

            remaining_size -= current_block_size;
        }
        file.clear(); 
    }

    file.close();

    if (total_blocks == 0) {
        throw std::runtime_error("No blocks were read from the file.");
    }

    return total_time / total_blocks; 
}



int main() {
    try {
        const std::string output_file = "results.csv";
        const std::string test_dir = "./test_files/";
        const size_t min_file_size = 512 * 1024 * 1024; 
        const size_t max_file_size = 28ULL * 1024 * 1024 * 1024; 
        const size_t step_factor = 2; 
        const size_t step_size = 512 * 1024 * 1024; 
        const int iterations = 5;

        create_directory(test_dir);

        std::ofstream results(output_file);
        if (!results.is_open()) {
            throw std::runtime_error("Failed to open results file");
        }

        results << "File Size (MB),Average Read Time (s)\n";

        for (size_t size = min_file_size; size <= max_file_size; size += step_size) {
            std::string filename = test_dir + "test_file_" + std::to_string(size / (1024 * 1024)) + "MB.bin";
            generate_file(filename, size);

        double avg_block_time = measure_block_access_time_reverse(filename, 1024 * 1024, iterations);

        std::cout << "File Size: " << (size / (1024 * 1024)) << " MB, Avg Block Access Time (Reverse): " << avg_block_time << " s\n";
        results << (size / (1024 * 1024)) << "," << avg_block_time << "\n";


            remove_file(filename); 
        }

        results.close();
        remove_directory(test_dir);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}







