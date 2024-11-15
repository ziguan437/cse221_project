#ifndef HELPER_H
#define HELPER_H

#include <vector>

double calculateMean(const std::vector<double>& times);
double calculateTrimmedMean(const std::vector<double>& times, double trimPercent);
double calculateStandardDeviation(const std::vector<double>& times, double mean); 
double calculateStdDevPercentage(const std::vector<double>& times, double mean);

#endif // HELPER_H

