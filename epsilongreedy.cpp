#include "main.h"

int selectActionEpsilonGreedy(std::vector<std::pair<double, int>> estimates, int k,
                              double epsilon)
{
    double maxEstimate = 0;
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
                maxEstimate = estimateValue;
            }
        }
    }
    else //Explorative move
    {
        index =  ( std::rand() % ( k ) );
    }
    return index;
}


