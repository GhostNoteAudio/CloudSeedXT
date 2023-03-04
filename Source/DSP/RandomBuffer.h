#pragma once

#include <vector>
#include <stdint.h>

namespace Cloudseed
{
	class RandomBuffer
	{
	public:
		static std::vector<float> Generate(uint64_t seed, int count);
		static std::vector<float> Generate(uint64_t seed, int count, float crossSeed);
	};
}
