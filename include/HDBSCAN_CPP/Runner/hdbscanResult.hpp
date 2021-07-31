#ifndef TWM_HDBSCAN_CPP_RUNNER_hdbscanResult_H
#define TWM_HDBSCAN_CPP_RUNNER_hdbscanResult_H
#pragma once
#include <vector>
#include <HDBSCAN_CPP/HdbscanStar/outlierScore.hpp>
using namespace std;
class hdbscanResult
{
public:
	vector <int> labels;
	vector <outlierScore> outliersScores;
	vector <double> membershipProbabilities;
	bool hasInfiniteStability;
	hdbscanResult();
	hdbscanResult(vector<int> pLables, vector<outlierScore> pOutlierScores, vector <double> pmembershipProbabilities, bool pHsInfiniteStability);
};

#endif // TWM_HDBSCAN_CPP_RUNNER_hdbscanResult_H
