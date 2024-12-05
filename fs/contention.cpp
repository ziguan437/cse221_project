#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <chrono>
#include <fstream>
#include <random>
#include "helper.h" 

const int BLOCK_SIZE = 4096; 
const int NUM_BLOCKS = 1024; 
const std::string FILE_PREFIX = "testfile_"; 
const int ITERATIONS = 10; 

struct ThreadData {
    int fileIndex;
    double averageTimePerBlock;
};


void createTestFiles(int numFiles) {
    char buffer[BLOCK_SIZE];
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 255);

    for (int i = 0; i < numFiles; ++i) {
        std::string fileName = FILE_PREFIX + std::to_string(i) + ".bin";
        std::ofstream outFile(fileName, std::ios::binary);
        for (int j = 0; j < BLOCK_SIZE * NUM_BLOCKS; ++j) {
            buffer[j % BLOCK_SIZE] = distribution(generator);
            if (j % BLOCK_SIZE == BLOCK_SIZE - 1) {
                outFile.write(buffer, BLOCK_SIZE);
            }
        }
    }
}


DWORD WINAPI fileReadWorker(LPVOID param) {
    ThreadData* data = static_cast<ThreadData*>(param);
    std::string fileName = FILE_PREFIX + std::to_string(data->fileIndex) + ".bin";

    HANDLE file = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING,
                             FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening file: " << fileName << " Error code: " << GetLastError() << "\n";
        return 1;
    }

    char* buffer = static_cast<char*>(_aligned_malloc(BLOCK_SIZE, BLOCK_SIZE));
    DWORD bytesRead;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_BLOCKS; ++i) {
        if (!ReadFile(file, buffer, BLOCK_SIZE, &bytesRead, NULL) || bytesRead != BLOCK_SIZE) {
            std::cerr << "Error reading file: " << fileName << " Error code: " << GetLastError() << "\n";
            CloseHandle(file);
            _aligned_free(buffer);
            return 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    CloseHandle(file);
    _aligned_free(buffer);


    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    data->averageTimePerBlock = static_cast<double>(totalDuration) / NUM_BLOCKS;

    return 0;
}

int main() {
    const int maxProcesses = 16; 
    const int numFiles = maxProcesses;

    createTestFiles(numFiles);

    for (int processCount = 1; processCount <= maxProcesses; ++processCount) {
        std::vector<double> averageTimes;

        for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
            std::vector<HANDLE> processHandles(processCount);
            std::vector<ThreadData> threadData(processCount);

            for (int i = 0; i < processCount; ++i) {
                threadData[i].fileIndex = i;
                processHandles[i] = CreateThread(NULL, 0, fileReadWorker, &threadData[i], 0, NULL);
                if (processHandles[i] == NULL) {
                    std::cerr << "Error creating thread. Error code: " << GetLastError() << "\n";
                    return 1;
                }
            }

            WaitForMultipleObjects(processCount, processHandles.data(), TRUE, INFINITE);

            for (int i = 0; i < processCount; ++i) {
                averageTimes.push_back(threadData[i].averageTimePerBlock);
                CloseHandle(processHandles[i]);
            }
        }

        double mean = calculateMean(averageTimes);
        double stddev = calculateStandardDeviation(averageTimes, mean);

        std::cout << "Processes: " << processCount << " - Mean: " << mean
                  << " microseconds, Std Dev: " << stddev << " microseconds\n";
    }

    for (int i = 0; i < numFiles; ++i) {
        std::string fileName = FILE_PREFIX + std::to_string(i) + ".bin";
        DeleteFile(fileName.c_str());
    }

    return 0;
}


