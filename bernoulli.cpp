#include "main.h"

std::vector<double> generateBernoulliDistribution(int k)
{
    std::vector<double> bernoulliDistribution;
    for (int i = 0; i < k; ++i)
    {
        double randomValue = ((double) rand() / (RAND_MAX)); //Value between 0 and 1
        bernoulliDistribution.push_back(randomValue);
    }
    return bernoulliDistribution;
}

//calculates if it is within it probability if so
//give reward of 1, else give reward of 0;
double calculateRewardBernoulli(double probability)
{
    double randomValue = ((double) rand() / (RAND_MAX));
    if (randomValue <= probability)
        return 1;

    return 0;
}

