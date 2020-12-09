#include "main.h"

enum Algorithm
{
    epsilon_greedy,
    ucb,
    gradient
};

std::vector<std::pair<double, int>> generateEstimates(int k)
{
    std::vector<std::pair<double, int>> estimates;
    std::pair<double, int> pair (4, 2);
    for (int i = 0; i < k; ++i)
        estimates.push_back(pair);

    return estimates;
}

double calculateEstimatedReward(double oldEstimate, int steps, double newReward)
{
    return oldEstimate + 1/double(steps) * (newReward - oldEstimate);
}

void debug(std::vector<double> averageRewardPerRun, std::vector<double> averageOptimalActionPerRun, double totalReward, int countOptimalActionTaken,
           int nRuns)
{
    /*std::cout << countOptimalActionTaken/nRuns << "\n";
    std::cout << "averageReward: " << totalReward << "\n";

    for (unsigned int i = 0; i < averageRewardPerRun.size(); ++i)
        std::cout << averageRewardPerRun[i] << ", ";*/
    //for (unsigned int i = 0; i < averageOptimalActionPerRun.size(); ++i)
        //std::cout << averageOptimalActionPerRun[i] << ", ";
    std::cout << "optimal action chosen: " << averageOptimalActionPerRun[averageOptimalActionPerRun.size()-1] << "\n";
    std::cout << "total reward: " << totalReward << "\n";
}

std::vector<double> createVectorDoubleWithElements(const int nElements)
{
    std::vector<double> averageRewardPerRun;
    for (int i = 0; i < nElements; ++i)
        averageRewardPerRun.push_back(0);
    return averageRewardPerRun;
}

std::vector<int> createVectorIntWithElements(const int nElements)
{
    std::vector<int> vector;
    for (int i = 0; i < nElements; ++i)
        vector.push_back(0);
    return vector;
}

void createFile(std::vector<double> value, std::string fileName, std::string yLabel)
{
    fileName += ".csv";
    std::ofstream myFile(fileName);

    myFile << "step" << "," << yLabel << "\n";
    for (unsigned int i = 0; i < value.size(); ++i)
    {
        myFile << i << ",";
        myFile << value[i] << "\n";
    }

    myFile.close();

}

int determineOptimalAction(std::vector<double> values)
{
    int optimalAction = 0;
    double highestValue = 0;
    for (size_t i = 0; i < values.size(); ++i)
    {
        if (values[i] > highestValue)
        {
            highestValue = values[i];
            optimalAction = i;
        }
    }
    return optimalAction;
}

int selectAction(Algorithm algorithm, std::vector<double> probabilities, std::vector<std::pair<double, int>> &estimates,
                 int k, double epsilon, int t, double c)
{
    switch (algorithm)
    {
        case epsilon_greedy:
            return selectActionEpsilonGreedy(estimates, k, epsilon);
        case ucb:
            return selectActionUCB(estimates, t, k, c);
        break;
        case gradient:
            return selectActionGradient(probabilities, k);
        break;
    }
    return 1;
}

void performActionSelections(std::vector<std::pair<double, int>> estimates, double &totalReward,
                             std::vector<double> &averageRewardPerStep, std::vector<int> &optimalActionPerStep,
                             int &countOptimalAction, int optimalAction, double alpha, double c,
                             int k, std::vector<double> meanGaussianValues, std::vector<double> bernoulliDistribution,
                             int nActionSelections,  int distribution, Algorithm algorithm, double epsilon)
{
    std::vector<double> preference = createVectorDoubleWithElements(k); //Also for gradient action selection
    for (int i = 0; i < nActionSelections; ++i)
    {
        std::vector<double> probabilities = calculateProbabilitiesGradient(preference, k);

        int action = selectAction(algorithm, probabilities, estimates, k, epsilon, i, c);

        if (action == optimalAction)
            countOptimalAction += 1;

        optimalActionPerStep[i] = countOptimalAction;

        double estimatedReward;
        double newReward = 0;

        if (distribution == 1)
        {
            newReward = calculateRewardGaussian(meanGaussianValues[action]);
            estimatedReward = calculateEstimatedReward(estimates[action].first,
                                                       estimates[action].second,
                                                       newReward);
        }
        else
        {
            newReward = calculateRewardBernoulli(bernoulliDistribution[action]);
            estimatedReward =  calculateEstimatedReward(estimates[action].first,
                                                        estimates[action].second,
                                                        newReward);
        }


        estimates[action].first = estimatedReward;
        estimates[action].second += 1;

        preference = updatePreferenceGradient(preference, estimates, probabilities, newReward, action, k, alpha);

        totalReward += newReward;

        averageRewardPerStep[i] = (totalReward/(i+1)); //change this so that it doesn't pushback
    }
}

void runAlgorithm(std::vector<double> &averageRewardPerRun, std::vector<double> &averageOptimalActionPerRun, int k,
                  int nActionSelections, std::vector<double> meanGaussianValues, double alpha, double c,
                  std::vector<double> bernoulliDistribution, int optimalAction, int distribution,
                  Algorithm algorithm, double &totalReward, int &countOptimalAction, double epsilon)
{
    totalReward = 0;
    std::vector<double> averageRewardPerStep = createVectorDoubleWithElements(nActionSelections);
    std::vector<int> optimalActionPerStep = createVectorIntWithElements(nActionSelections);
    std::vector<std::pair<double, int>> estimates = generateEstimates(k);

    performActionSelections(estimates, totalReward, averageRewardPerStep, optimalActionPerStep, countOptimalAction,
                            optimalAction, alpha, c, k, meanGaussianValues, bernoulliDistribution,
                            nActionSelections, distribution, algorithm, epsilon);

    for (int j = 0; j < nActionSelections; ++j)
    {
        averageRewardPerRun[j] += averageRewardPerStep[j];
        averageOptimalActionPerRun[j] += (double)optimalActionPerStep[j]/ (double)j;
    }

}

void runAlgorithmLoop()
{
    Algorithm algorithm = epsilon_greedy;
    int distribution = 2;
    const int nActionSelections = 10000;
    const int nRuns = 2000;
    const int k = 10;

    double epsilon = 0.08;
    double c = 0.5;
    double alpha = 0.09;

    int countOptimalActionOverRuns=0;
    double totalReward = 0;
    std::vector<double> totalRewardPerRun;
    std::vector<double> averageRewardPerRun = createVectorDoubleWithElements(nActionSelections);
    std::vector<double> averageOptimalActionPerRun = createVectorDoubleWithElements(nActionSelections);

    for (int i = 0; i < nRuns; ++i)
    {
        int countOptimalAction = 0;
        std::vector<double> meanGaussianValues = generateMeanValuesGaussian(k);
        std::vector<double> bernoulliDistribution = generateBernoulliDistribution(k);

        int optimalAction;
        if (distribution == 1)
            optimalAction = determineOptimalAction(meanGaussianValues);
        else
            optimalAction = determineOptimalAction(bernoulliDistribution);

        runAlgorithm(averageRewardPerRun, averageOptimalActionPerRun, k, nActionSelections, meanGaussianValues,
                     alpha, c, bernoulliDistribution, optimalAction, distribution,
                     algorithm, totalReward, countOptimalAction, epsilon);

        totalRewardPerRun.push_back(totalReward);
        countOptimalActionOverRuns += countOptimalAction;
    }

    for (int i = 0; i < nActionSelections; ++i)
    {
        averageRewardPerRun[i] = averageRewardPerRun[i]/ nRuns;
        averageOptimalActionPerRun[i] = averageOptimalActionPerRun[i]/ nRuns;
    }

    debug(averageRewardPerRun, averageOptimalActionPerRun, totalReward, countOptimalActionOverRuns, nRuns);

    createFile(averageRewardPerRun, "averageReward", "average_reward");
    createFile(averageOptimalActionPerRun, "optimalAction", "optimal_action");
    createFile(totalRewardPerRun, "totalReward", "total_reward");
}


int main()
{
    srand(time(0));
    runAlgorithmLoop();
}
