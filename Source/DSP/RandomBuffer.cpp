
#include <climits>
#include "RandomBuffer.h"
#include "LcgRandom.h"

namespace Cloudseed
{
	std::vector<float> RandomBuffer::Generate(uint64_t seed, int count)
	{
		LcgRandom rand(seed);
		std::vector<float> output;

		for (int i = 0; i < count; i++)
		{
			unsigned int val = rand.NextUInt();
			float fVal = val / (float)UINT_MAX;
			output.push_back(fVal);
		}

		return output;
	}

	std::vector<float> RandomBuffer::Generate(uint64_t seed, int count, float crossSeed)
	{
		auto seedA = seed;
		auto seedB = ~seed;
		auto seriesA = Generate(seedA, count);
		auto seriesB = Generate(seedB, count);

		std::vector<float> output;
		for (int i = 0; i < count; i++)
			output.push_back(seriesA[i] * (1 - crossSeed) + seriesB[i] * crossSeed);

		return output;
	}
}
