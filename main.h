#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <random>
#include <time.h>
#include <math.h>
#include <fstream>
#include <iomanip>

int selectActionEpsilonGreedy(std::vector<std::pair<double, int>> estimates, int k,
                double epsilon);
std::vector<double> generateBernoulliDistribution(int k);
std::vector<double> generateMeanValuesGaussian(int k);
double calculateRewardGaussian(double mean);
double calculateRewardBernoulli(double probability);
int selectActionUCB(std::vector<std::pair<double, int>> estimates, int t, int k);

#endif // MAIN_H
