#ifndef TWM_HDBSCAN_CPP_UTILS_bitSet_H
#define TWM_HDBSCAN_CPP_UTILS_bitSet_H
#pragma once
#include <vector>
class bitSet
{
private:
	std::vector<bool> _bits;
public:
	bool get(int pos);
	
	void set(int pos);

	void ensure(int pos);
};

#endif // TWM_HDBSCAN_CPP_UTILS_bitSet_H
