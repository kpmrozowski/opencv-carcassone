#ifndef TWM_HDBSCAN_CPP_RUNNER_hdbscanResult_H
#define TWM_HDBSCAN_CPP_RUNNER_hdbscanResult_H
#pragma once
#include <vector>
#include <HDBSCAN_CPP/HdbscanStar/outlierScore.hpp>
class hdbscanResult
{
public:
	std::vector <int> labels;
	std::vector <outlierScore> outliersScores;
	std::vector <double> membershipProbabilities;
	bool hasInfiniteStability;
	hdbscanResult();
	hdbscanResult(std::vector<int> pLables, std::vector<outlierScore> pOutlierScores, std::vector <double> pmembershipProbabilities, bool pHsInfiniteStability);
};

#endif // TWM_HDBSCAN_CPP_RUNNER_hdbscanResult_H
