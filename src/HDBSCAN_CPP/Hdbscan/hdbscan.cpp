#include <HDBSCAN_CPP/Hdbscan/hdbscan.hpp>
#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
#include<map>
#include<cstdint>

std::string Hdbscan::getFileName() {
	return this->fileName;
}
/// <summary>
///	Loads the csv file as specified by the constructor.CSV
/// </summary>
/// <param name="numberOfalues">A List of attributes to be choosen</param>
/// <param name="skipHeader">Bool value to skip header or not</param>
/// <returns>1 if successful, 0 otherwise</returns>

int Hdbscan::loadCsv(int numberOfValues, bool skipHeader) {
	std::string  attribute;

	std::string line = "";

	int currentAttributes;
	std::vector<std::vector<double> > dataset;

	std::string fileName = this->getFileName();
	std::ifstream file(fileName, std::ios::in);
	if (!file)
		return 0;
	if (skipHeader) {
		getline(file, line);

	}
	while (getline(file, line)) {      //Read through each line
		std::stringstream s(line);
		std::vector<double> row;
		currentAttributes = numberOfValues;
		while (getline(s, attribute, ',') && currentAttributes != 0) {
			row.push_back(stod(attribute));
			currentAttributes--;
		}
		dataset.push_back(row);

	}
	this->dataset = dataset;
	return 1;
}

void Hdbscan::execute(int minPoints, int minClusterSize, std::string distanceMetric) {
	//Call The Runner Class here
	hdbscanRunner runner;
	hdbscanParameters parameters;
	uint32_t noisyPoints = 0;
	std::set<int> numClustersSet;
	std::map<int, int> clustersMap;
	std::vector<int> normalizedLabels;

	parameters.dataset = this->dataset;
	parameters.minPoints = minPoints;
	parameters.minClusterSize = minClusterSize;
	parameters.distanceFunction = distanceMetric;
    	this->result = runner.run(parameters);
	this->labels_ = result.labels;
	this->outlierScores_ = result.outliersScores;
	for (uint32_t i = 0; i < result.labels.size(); i++) {
		if (result.labels[i] == 0) {
			noisyPoints++;
		}
		else {
			numClustersSet.insert(result.labels[i]);
		}
	}
	this->numClusters_ = numClustersSet.size();
	this->noisyPoints_ = noisyPoints;
	int iNdex = 1;
	for (auto it = numClustersSet.begin(); it != numClustersSet.end(); it++) {
		clustersMap[*it] = iNdex++;
	}
	for (int i = 0; i < labels_.size(); i++) {
		if (labels_[i] != 0)
			normalizedLabels.push_back(clustersMap[labels_[i]]);
		else if (labels_[i] == 0) {
			normalizedLabels.push_back(-1);
		}

	}
	this->normalizedLabels_ = normalizedLabels;
	this->membershipProbabilities_ = result.membershipProbabilities;
}

void Hdbscan::displayResult() {
	hdbscanResult result = this->result;
	uint32_t numClusters = 0;

	std::cout << "HDBSCAN clustering for " << this->dataset.size() << " objects." << std::endl;

	for (uint32_t i = 0; i < result.labels.size(); i++) {
		std::cout << result.labels[i] << " ";
	}

	std::cout << std::endl << std::endl;

	std::cout << "The Clustering contains " << this->numClusters_ << " clusters with " << this->noisyPoints_ << " noise Points." << std::endl;

}
