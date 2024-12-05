#ifndef HELPER_H
#define HELPER_H

#include <vector>

double calculateMean(const std::vector<double>& times);
double calculateTrimmedMean(const std::vector<double>& times, double trimPercent);
double calculateStandardDeviation(const std::vector<double>& times, double mean); 
double calculateStdDevPercentage(const std::vector<double>& times, double mean);
std::vector<double> removeOutliers(const std::vector<double>& values);

#endif 

