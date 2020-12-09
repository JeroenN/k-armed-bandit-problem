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
int selectActionGradient(std::vector<double> probabilities, int k);
int selectActionUCB(std::vector<std::pair<double, int>> estimates, int t, int k, double c);
std::vector<double> updatePreferenceGradient(std::vector<double> preference, std::vector<std::pair<double, int>> estimates,
                                             std::vector<double> probabilities, double reward, int action, int k, double alpha);
std::vector<double> calculateProbabilitiesGradient(std::vector<double> preference, int k);
std::vector<double> generateBernoulliDistribution(int k);
std::vector<double> generateMeanValuesGaussian(int k);
double calculateRewardGaussian(double mean);
double calculateRewardBernoulli(double probability);


#endif // MAIN_H
