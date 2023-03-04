#pragma once

#include <vector>
#include <memory>
#include <array>
#include <cmath>
#include "Utils.h"
#include "RandomBuffer.h"

namespace Cloudseed
{
	class MultitapDelay
	{
	public:
		static const int MaxTaps = 256;
		static const int DelayBufferSize = 192000 * 2;

	private:
		float delayBuffer[DelayBufferSize] = { 0 };
		float output[BUFFER_SIZE] = { 0 };

		float tapGains[MaxTaps] = { 0 };
		float tapPosition[MaxTaps] = { 0 };

		std::vector<float> seedValues;

		int writeIdx;
		int seed;
		float crossSeed;
		int count;
		float lengthSamples;
		float decay;

	public:
		MultitapDelay()
		{
			writeIdx = 0;
			seed = 0;
			crossSeed = 0.0;
			count = 1;
			lengthSamples = 1000;
			decay = 1.0;
			
			UpdateSeeds();
		}

		void SetSeed(int seed)
		{
			this->seed = seed;
			UpdateSeeds();
		}

		void SetCrossSeed(float crossSeed)
		{
			this->crossSeed = crossSeed;
			UpdateSeeds();
		}

		float* GetOutput()
		{
			return output;
		}

		void SetTapCount(int tapCount)
		{
			if (tapCount < 1) tapCount = 1;
			count = tapCount;
			Update();
		}

		void SetTapLength(int tapLengthSamples)
		{
			if (tapLengthSamples < 10) tapLengthSamples = 10;
			lengthSamples = tapLengthSamples;
			Update();
		}

		void SetTapDecay(float tapDecay)
		{
			decay = tapDecay;
		}

		void Process(float* input, int sampleCount)
		{
			float lengthScaler = lengthSamples / (float)count;
			float totalGain = 1.0 / std::sqrtf(1 + count);

			for (int i = 0; i < sampleCount; i++)
			{
				delayBuffer[writeIdx] = input[i];
				output[i] = 0;

				for (int j = 0; j < count; j++)
				{
					float offset = tapPosition[j] * lengthScaler;
					float decayEffective = std::expf(-offset / lengthSamples * 3.3) * decay + (1-decay);
					int readIdx = writeIdx - (int)offset;
					if (readIdx < 0) readIdx += DelayBufferSize;

					output[i] += delayBuffer[readIdx] * tapGains[j] * decayEffective * totalGain;
				}

				writeIdx = (writeIdx + 1) % DelayBufferSize;
			}
		}

		void ClearBuffers()
		{
			Utils::ZeroBuffer(delayBuffer, DelayBufferSize);
			Utils::ZeroBuffer(output, BUFFER_SIZE);
		}


	private:
		void Update()
		{
			int s = 0;
			auto rand = [&]() {return seedValues[s++]; };

			for (int i = 0; i < MaxTaps; i++)
			{
				float phase = rand() < 0.5 ? 1 : -1;
				tapGains[i] = Utils::DB2Gainf(-20 + rand() * 20) * phase;
				tapPosition[i] = i + rand();
			}
		}

		void UpdateSeeds()
		{
			this->seedValues = RandomBuffer::Generate(seed, MaxTaps * 3, crossSeed);
			Update();
		}
	};
}
