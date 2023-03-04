#pragma once

#include <vector>


class RandomBuffer
{
public:
	static std::vector<float> Generate(long long seed, int count);
	static std::vector<float> Generate(long long seed, int count, float crossSeed);
};


