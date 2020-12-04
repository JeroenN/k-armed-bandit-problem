#include "main.h"

int selectEachActionOnce(std::vector<std::pair<double, int>> estimates, int k)
{
    for (int i = 0; i < k; ++i)
    {
        if (estimates[i].second == 0)
        {
            std::cout << "test";
            return i;
        }
    }
    return -1;
}

int selectActionUCB(std::vector<std::pair<double, int>> estimates, int t, int k)
{
    //At = argmax(Qt(a) + c *sqrt(ln(t) / Nt(a))
    double maxEstimate = 0;
    int action = 0;
    int c = 2;
    int preAction = selectEachActionOnce(estimates, k);
    if (preAction != -1)
        return action;

    for (int i = 0; i < k; ++i)
    {
        double estimateValue = estimates[i].first + c * sqrt (log (t) / estimates[i].second);
        if (estimateValue > maxEstimate)
        {
            maxEstimate = estimateValue;
            action = i;
        }
    }
    return action;
}



