#pragma once

#include "ModulatedDelay.h"
#include "Utils.h"
#include <stdint.h>

namespace Cloudseed
{
	class ModulatedDelay
	{
	private:

		static const int ModulationUpdateRate = 8;
		static const int DelayBufferSize = 192000 * 2;

		float delayBuffer[DelayBufferSize] = { 0 };
		int writeIndex;
		int readIndexA;
		int readIndexB;
		uint64_t samplesProcessed;

		float modPhase;
		float gainA;
		float gainB;

	public:
		int SampleDelay;

		float ModAmount;
		float ModRate;

		ModulatedDelay()
		{
			writeIndex = 0;
			readIndexA = 0;
			readIndexB = 0;
			samplesProcessed = 0;

			modPhase = 0.01 + 0.98 * (std::rand() / (float)RAND_MAX);
			gainA = 0;
			gainB = 0;

			SampleDelay = 100;
			ModAmount = 0.0;
			ModRate = 0.0;

			Update();
		}

		void Process(float* input, float* output, int bufSize)
		{
			for (int i = 0; i < bufSize; i++)
			{
				if (samplesProcessed % ModulationUpdateRate == 0)
					Update();

				delayBuffer[writeIndex] = input[i];
				output[i] = delayBuffer[readIndexA] * gainA + delayBuffer[readIndexB] * gainB;

				writeIndex++;
				readIndexA++;
				readIndexB++;
				if (writeIndex >= DelayBufferSize) writeIndex -= DelayBufferSize;
				if (readIndexA >= DelayBufferSize) readIndexA -= DelayBufferSize;
				if (readIndexB >= DelayBufferSize) readIndexB -= DelayBufferSize;
				samplesProcessed++;
			}
		}

		void ClearBuffers()
		{
			Utils::ZeroBuffer(delayBuffer, DelayBufferSize);
		}


	private:
		void Update()
		{
			modPhase += ModRate * ModulationUpdateRate;
			if (modPhase > 1)
				modPhase = std::fmod(modPhase, 1.0);

			auto mod = std::sinf(modPhase * 2 * M_PI);
			auto totalDelay = SampleDelay + ModAmount * mod;

			auto delayA = (int)totalDelay;
			auto delayB = (int)totalDelay + 1;

			auto partial = totalDelay - delayA;

			gainA = 1 - partial;
			gainB = partial;

			readIndexA = writeIndex - delayA;
			readIndexB = writeIndex - delayB;
			if (readIndexA < 0) readIndexA += DelayBufferSize;
			if (readIndexB < 0) readIndexB += DelayBufferSize;
		}
	};
}
