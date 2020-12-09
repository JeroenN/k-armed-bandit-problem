#include "main.h"

//SoftMax function, adds up to one
std::vector<double> calculateProbabilitiesGradient(std::vector<double> preference, int k)
{
    std::vector<double> probabilities;

    double probability;
    double sumOfAllPrefrences = 0;

    for (int i = 0; i < k; ++i)
        sumOfAllPrefrences += exp (preference[i]);

    for (int i = 0; i < k; ++i)
    {
        probability = exp (preference[i]) / sumOfAllPrefrences;
        probabilities.push_back(probability);
    }
    return probabilities;
}

int selectActionGradient(std::vector<double> probabilities, int k)
{
    double randomValue = ((double) rand() / (RAND_MAX));
    double cutoff = 0;
    for (int i = 0; i < k; ++i)
    {
        cutoff += probabilities[i];
        if (randomValue <= cutoff)
            return i;
    }
    return k-1; //happens when randomvalue is equal to 1
}

std::vector<double> updatePreferenceGradient(std::vector<double> preference, std::vector<std::pair<double, int>> estimates,
                                             std::vector<double> probabilities, double reward, int action, int k, double alpha)
{
    preference[action] = preference[action] + alpha * (reward - estimates[action].first) * (1 - probabilities[action]);
    for (int i = 0; i < k; ++i)
    {
        if (i != action)
            preference[i] = preference[i] - alpha * (reward - estimates[i].first) * probabilities[i];
    }
    return preference;
}
//Ht+1(a) =Ht(a)−α(Rt− ̄Rt)πt(a)
//e^Ht(a)/∑nb=1 e^Ht(b) = πt(a)
