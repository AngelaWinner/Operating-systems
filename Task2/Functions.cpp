#include <iostream>
#include <vector>

void min_max1(int& minValue, int& maxValue, std::vector<int> myVector)
{
	maxValue = myVector[0];
	minValue = myVector[0];

	for (int i = 0; i < myVector.size(); i++)
	{
		if (myVector[i] <= minValue)
		{
			minValue = myVector[i];
		}

		if (myVector[i] >= maxValue)
		{
			maxValue = myVector[i];
		}
	}
}


void average1(int& average, std::vector<int> myVector)
{
	for (int i = 0; i < myVector.size(); i++)
	{
		average += (myVector[i] / myVector.size());
	}
}