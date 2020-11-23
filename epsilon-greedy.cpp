#include <iostream>
#include <random>
#include <time.h>

std::vector<int> generateMeanValues(std::default_random_engine generator, int k)
{
    std::vector<int> meanValues;
    std::uniform_int_distribution<> distribution(0, 5);
    for (int i = 0; i < k; ++i)
    {
        meanValues.push_back(distribution(generator));
    }
    return meanValues;
}

double calculateReward(int mean)
{

    static std::default_random_engine generator;
    std::normal_distribution<double> distribution(double(mean),2.0);
    return distribution(generator);
}

double calculateEstimatedReward(double oldEstimate, int steps, int mean)
{
    return oldEstimate + 1/double(steps) * (calculateReward(mean) - oldEstimate);
}

std::vector<std::pair<double, int>> generateEstimates(int k)
{
    std::vector<std::pair<double, int>> estimates;
    std::pair<double, int> pair (0, 1);
    for (int i = 0; i < k; ++i)
        estimates.push_back(pair);

    return estimates;
}

int selectLever(std::vector<std::pair<double, int>> estimates, int k,
                double epsilon)
{
    int maxEstimate = 0;
    int index = 0;
    double randomValue = ((double) rand() / (RAND_MAX)); //Value between 0 and 1

    if (randomValue > epsilon)  //explotiative move
    {
        for (int i = 0; i < k; ++i)
        {
            double estimateValue = estimates[i].first;
            if (estimateValue > maxEstimate)
            {
                index = i;
                estimateValue = estimates[0].first;
            }
        }
    }
    else //Explorative move
    {
        index =  ( std::rand() % ( 4 + 1 ) );
    }

    return index;
}

int main()
{
    const int k = 5;
    const double epsilon = 0.1;
    std::default_random_engine generator;
    std::vector<std::pair<double, int>> estimates = generateEstimates(k);
    std::vector<int> meanValues = generateMeanValues(generator, k);

    for (int i = 0; i < 10000; ++i)
    {
        int lever = selectLever(estimates, k, epsilon);
        estimates[lever].first = calculateEstimatedReward(estimates[lever].first,
                                                          estimates[lever].second, meanValues[lever]);
        estimates[lever].second += 1;
    }

    for (int i = 0; i < k; ++i)
    {
        std::cout << "estimated value of " << i << " lever: " << estimates[i].first << "\n";
        std::cout << "amount of steps taken of " << i << " lever: " << estimates[i].second << "\n \n";
    }

    for (int i = 0; i < k; ++i)
        std::cout << i << ": " << meanValues[i] << "\n";

    return 0;
}
/*
    std::vector<int> vector;
    for (int i = 0; i < 10; ++i)
        vector.push_back(0);

    for (int i = 0; i <100; ++i)
    {
        int value = int(distribution(generator));
        for (unsigned int j = 0; j < vector.size(); ++j)
        {
            if (j == unsigned(value))
                vector[j] += 1;
        }
    }

    for (unsigned int i =0; i < vector.size(); ++i)
    {
        std::cout << i << ": ";
        for (int j = 0; j < vector[i]; ++j)
            std::cout << "*";

        std::cout << "\n";
    }
*/
