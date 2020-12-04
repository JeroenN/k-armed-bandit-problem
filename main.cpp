#include "main.h"

std::vector<std::pair<double, int>> generateEstimates(int k)
{
    std::vector<std::pair<double, int>> estimates;
    std::pair<double, int> pair (0, 1);
    for (int i = 0; i < k; ++i)
        estimates.push_back(pair);

    return estimates;
}

double calculateEstimatedReward(double oldEstimate, int steps, double newReward)
{
    return oldEstimate + 1/double(steps) * (newReward - oldEstimate);
}

void debug(std::vector<double> averageRewardPerRun, double totalReward, int countOptimalActionTaken,
           int nRuns)
{
    std::cout << countOptimalActionTaken/nRuns << "\n";
    std::cout << "averageReward: " << totalReward << "\n";

    for (unsigned int i = 0; i < averageRewardPerRun.size(); ++i)
        std::cout << averageRewardPerRun[i] << ", ";
}

std::vector<double> createVectorWithElements(const int nElements)
{
    std::vector<double> averageRewardPerRun;
    for (int i = 0; i < nElements; ++i)
        averageRewardPerRun.push_back(0);
    return averageRewardPerRun;
}

void createFileAverageReward(std::vector<double> averageRewardPerRun)
{
    std::ofstream myFile("averageReward.csv");

    myFile << "step" << "," << "average_reward" << "\n";
    for (unsigned int i = 0; i < averageRewardPerRun.size(); ++i)
    {
        myFile << i << ",";
        myFile << averageRewardPerRun[i] << "\n";
    }

    myFile.close();
}

int determineOptimalAction(std::vector<double> values)
{
    int optimalAction = 0;
    int highestValue = 0;
    for (unsigned int i = 0; i < values.size(); ++i)
    {
        if (values[i] > highestValue)
        {
            highestValue = values[i];
            optimalAction = i;
        }
    }
    return optimalAction;
}

int selectAction(int algorithm, std::vector<std::pair<double, int>> &estimates, int k, double epsilon, int t)
{
    switch (algorithm)
    {
        case 1:
            return selectActionEpsilonGreedy(estimates, k, epsilon);
        case 2:
            return selectActionUCB(estimates, t, k);
        break;
    }
    return 1;
}

void performActionSelections(std::vector<std::pair<double, int>> estimates, double &totalReward,
                             std::vector<double> &averageRewardPerStep, int &countOptimalActionTaken, int optimalAction,
                             int k, std::vector<double> meanGaussianValues, std::vector<double> bernoulliDistribution,
                             int nActionSelections,  int distribution, int algorithm, double epsilon)
{
    for (int i = 0; i < nActionSelections; ++i)
    {
        int action = selectAction(algorithm, estimates, k, epsilon, i);

        if (action == optimalAction)
            countOptimalActionTaken++;

        double estimatedReward;
        if (distribution == 1)
            estimatedReward = calculateEstimatedReward(estimates[action].first,
                                                       estimates[action].second,
                                                       calculateRewardGaussian(meanGaussianValues[action]));
        else
            estimatedReward =  calculateEstimatedReward(estimates[action].first,
                                                        estimates[action].second,
                                                        calculateRewardBernoulli(bernoulliDistribution[action]));

        estimates[action].first = estimatedReward;
        estimates[action].second += 1;

        totalReward += estimatedReward;

        averageRewardPerStep[i] = (totalReward/(i+1)); //change this so that it doesn't pushback
    }
}

void runAlgorithm(std::vector<double> &averageRewardPerRun, int k, int nActionSelections, std::vector<double> meanGaussianValues,
                  std::vector<double> bernoulliDistribution, int optimalAction, int distribution,
                  int algorithm, double &totalReward, int &countOptimalActionTaken)
{
    totalReward = 0;
    std::vector<double> averageRewardPerStep = createVectorWithElements(nActionSelections);
    std::vector<std::pair<double, int>> estimates = generateEstimates(k);
    double epsilon = 0.1;

    performActionSelections(estimates, totalReward, averageRewardPerStep, countOptimalActionTaken,
                            optimalAction, k, meanGaussianValues, bernoulliDistribution,
                            nActionSelections, distribution, algorithm, epsilon);

    for (int j = 0; j < nActionSelections; ++j)
        averageRewardPerRun[j] += averageRewardPerStep[j];
}

void runAlgorithmParameters()
{
    int algorithm = 2;
    int distribution = 2;
    const int nActionSelections = 10000;
    const int nRuns = 1000;
    const int k = 10;
    int countOptimalActionTaken = 0;
    double totalReward;
    std::vector<double> averageRewardPerRun = createVectorWithElements(nActionSelections);

    for (int i = 0; i < nRuns; ++i)
    {
        std::vector<double> meanGaussianValues = generateMeanValuesGaussian(k);
        std::vector<double> bernoulliDistribution = generateBernoulliDistribution(k);

        int optimalAction;
        if (distribution == 1)
            optimalAction = determineOptimalAction(meanGaussianValues);
        else
            optimalAction = determineOptimalAction(bernoulliDistribution);

        runAlgorithm(averageRewardPerRun, k, nActionSelections, meanGaussianValues, bernoulliDistribution, optimalAction, distribution,
                     algorithm, totalReward, countOptimalActionTaken);
    }

    for (int i = 0; i < nActionSelections; ++i)
        averageRewardPerRun[i] = averageRewardPerRun[i]/nRuns;

    debug(averageRewardPerRun, totalReward, countOptimalActionTaken, nRuns);

    createFileAverageReward(averageRewardPerRun);
}


int main()
{
    runAlgorithmParameters();
    return 0;
}
