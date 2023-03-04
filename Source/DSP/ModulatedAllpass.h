#pragma once

#include "ModulatedAllpass.h"
#include "Utils.h"
#include <cmath>

namespace Cloudseed
{
	class ModulatedAllpass
	{
	public:
		static const int DelayBufferSize = 19200; // 100ms at 192Khz
		static const int ModulationUpdateRate = 8;

	private:
		float delayBuffer[DelayBufferSize] = { 0 };
		float output[BUFFER_SIZE] = { 0 };
		int index;
		uint64_t samplesProcessed;

		float modPhase;
		int delayA;
		int delayB;
		float gainA;
		float gainB;

	public:

		int SampleDelay;
		float Feedback;
		float ModAmount;
		float ModRate;

		bool InterpolationEnabled;
		bool ModulationEnabled;

		ModulatedAllpass()
		{
			index = DelayBufferSize - 1;
			samplesProcessed = 0;

			modPhase = 0.01 + 0.98 * std::rand() / (float)RAND_MAX;
			delayA = 0;
			delayB = 0;
			gainA = 0;
			gainB = 0;

			SampleDelay = 100;
			Feedback = 0.5;
			ModAmount = 0.0;
			ModRate = 0.0;

			InterpolationEnabled = true;
			ModulationEnabled = true;
			Update();
		}

		inline float* GetOutput()
		{
			return output;
		}

		void ClearBuffers()
		{
			Utils::ZeroBuffer(delayBuffer, DelayBufferSize);
			Utils::ZeroBuffer(output, BUFFER_SIZE);
		}

		void Process(float* input, int sampleCount)
		{
			if (ModulationEnabled)
				ProcessWithMod(input, sampleCount);
			else
				ProcessNoMod(input, sampleCount);
		}

	private:
		void ProcessNoMod(float* input, int sampleCount)
		{
			auto delayedIndex = index - SampleDelay;
			if (delayedIndex < 0) delayedIndex += DelayBufferSize;

			for (int i = 0; i < sampleCount; i++)
			{
				auto bufOut = delayBuffer[delayedIndex];
				auto inVal = input[i] + bufOut * Feedback;

				delayBuffer[index] = inVal;
				output[i] = bufOut - inVal * Feedback;

				index++;
				delayedIndex++;
				if (index >= DelayBufferSize) index -= DelayBufferSize;
				if (delayedIndex >= DelayBufferSize) delayedIndex -= DelayBufferSize;
				samplesProcessed++;
			}
		}

		void ProcessWithMod(float* input, int sampleCount)
		{
			for (int i = 0; i < sampleCount; i++)
			{
				if (samplesProcessed % ModulationUpdateRate == 0)
					Update();

				float bufOut;

				if (InterpolationEnabled)
				{
					int idxA = index - delayA;
					int idxB = index - delayB;
					idxA += DelayBufferSize * (idxA < 0); // modulo
					idxB += DelayBufferSize * (idxB < 0); // modulo

					bufOut = delayBuffer[idxA] * gainA + delayBuffer[idxB] * gainB;
				}
				else
				{
					int idxA = index - delayA;
					idxA += DelayBufferSize * (idxA < 0); // modulo
					bufOut = delayBuffer[idxA];
				}

				auto inVal = input[i] + bufOut * Feedback;
				delayBuffer[index] = inVal;
				output[i] = bufOut - inVal * Feedback;

				index++;
				if (index >= DelayBufferSize) index -= DelayBufferSize;
				samplesProcessed++;
			}
		}

		inline float Get(int delay)
		{
			int idx = index - delay;
			if (idx < 0)
				idx += DelayBufferSize;

			return delayBuffer[idx];
		}

		void Update()
		{
			modPhase += ModRate * ModulationUpdateRate;
			if (modPhase > 1)
				modPhase = std::fmod(modPhase, 1.0);

			auto mod = std::sinf(modPhase * 2 * M_PI);

			if (ModAmount >= SampleDelay) // don't modulate to negative value
				ModAmount = SampleDelay - 1;


			auto totalDelay = SampleDelay + ModAmount * mod;

			if (totalDelay <= 0) // should no longer be required
				totalDelay = 1;

			delayA = (int)totalDelay;
			delayB = (int)totalDelay + 1;

			auto partial = totalDelay - delayA;

			gainA = 1 - partial;
			gainB = partial;
		}
	};
}
