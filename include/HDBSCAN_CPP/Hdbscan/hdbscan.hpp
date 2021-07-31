#ifndef TWM_HDBSCAN_CPP_HDBSCAN_H
#define TWM_HDBSCAN_CPP_HDBSCAN_H
#pragma once
#include <string>
#include <vector>
#include <HDBSCAN_CPP/Runner/hdbscanRunner.hpp>
#include <HDBSCAN_CPP/Runner/hdbscanParameters.hpp>
#include <HDBSCAN_CPP/Runner/hdbscanResult.hpp>
#include <HDBSCAN_CPP/HdbscanStar/outlierScore.hpp>

class Hdbscan {

private:

	std::string fileName;

	hdbscanResult result;

public:

	std::vector<std::vector<double>> dataset;

	std::vector<int> labels_;

	std::vector<int> normalizedLabels_;

	std::vector<outlierScore>outlierScores_;

	std::vector <double> membershipProbabilities_;

	uint32_t noisyPoints_;

	uint32_t numClusters_;

	Hdbscan(std::string readFileName) {
		fileName = readFileName;
	}

	// Hdbscan(std::vector<std::vector<double>> _dataset) : dataset(_dataset) {}

	Hdbscan() {}

	std::string getFileName();
			   
	int loadCsv(int numberOfValues, bool skipHeader=false);

	void execute(int minPoints, int minClusterSize, std::string distanceMetric);

	void displayResult();


};

#endif // TWM_HDBSCAN_CPP_HDBSCAN_H
