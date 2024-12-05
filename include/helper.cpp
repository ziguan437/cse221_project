#include "helper.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

// Calculate the mean of a vector of times
double calculateMean(const std::vector<double>& times) {
    if (times.empty()) return 0.0;
    return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

// Calculate the trimmed mean of a vector of times
double calculateTrimmedMean(const std::vector<double>& times, double trimPercent) {
    if (times.empty() || trimPercent < 0.0 || trimPercent >= 0.5) return 0.0;

    std::vector<double> sortedTimes = times;
    std::sort(sortedTimes.begin(), sortedTimes.end());

    size_t trimCount = static_cast<size_t>(sortedTimes.size() * trimPercent);
    if (2 * trimCount >= sortedTimes.size()) return 0.0; 

    auto start = sortedTimes.begin() + trimCount;
    auto end = sortedTimes.end() - trimCount;
    double sum = std::accumulate(start, end, 0.0);
    return sum / std::distance(start, end);
}

double calculateStandardDeviation(const std::vector<double>& times, double mean) {
    if (times.empty()) {
        throw std::invalid_argument("The input vector is empty.");
    }

    double sum = 0.0;
    for (double time : times) {
        sum += (time - mean) * (time - mean);
    }
    double variance = sum / times.size();
    return std::sqrt(variance);
}

// Calculate the standard deviation as a percentage of the mean
double calculateStdDevPercentage(const std::vector<double>& times, double mean) {
    if (times.empty() || mean == 0.0) return 0.0;

    double sum = 0.0;
    for (double time : times) {
        sum += (time - mean) * (time - mean);
    }
    double variance = sum / times.size();
    double stddev = std::sqrt(variance);
    return (stddev / mean) * 100.0; 
}

std::vector<double> removeOutliers(const std::vector<double>& values) {
    if (values.size() < 4) {
        return values;
    }

    std::vector<double> sortedValues = values;
    std::sort(sortedValues.begin(), sortedValues.end());

    size_t n = sortedValues.size();
    double Q1 = sortedValues[n / 4];
    double Q3 = sortedValues[(3 * n) / 4];

    double IQR = Q3 - Q1;

    double lowerBound = Q1 - 1.5 * IQR;
    double upperBound = Q3 + 1.5 * IQR;

    std::vector<double> filteredValues;
    for (double val : values) {
        if (val >= lowerBound && val <= upperBound) {
            filteredValues.push_back(val);
        }
    }

    return filteredValues;
}

