#include "main.h"

std::vector<double> generateMeanValuesGaussian(int k)
{
    std::vector<double> meanValues;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0, 3.0);
    for (int i = 0; i < k; ++i)
    {
        meanValues.push_back(distribution(generator));
    }
    return meanValues;
}

double calculateRewardGaussian(double mean)
{
    static std::default_random_engine generator(time(0));
    std::normal_distribution<double> distribution(mean, 3.0);
    return distribution(generator);
}


