#ifndef TWM_HDBSCAN_CPP_RUNNER_hdbscanRunner_H
#define TWM_HDBSCAN_CPP_RUNNER_hdbscanRunner_H
#pragma once
#include <HDBSCAN_CPP/HdbscanStar/outlierScore.hpp>
#include <HDBSCAN_CPP/Runner/hdbscanResult.hpp>
#include <HDBSCAN_CPP/Runner/hdbscanParameters.hpp>
class hdbscanRunner
{
public:
	static hdbscanResult run(hdbscanParameters parameters);
};

#endif // TWM_HDBSCAN_CPP_RUNNER_hdbscanRunner_H
