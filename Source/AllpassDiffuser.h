
#pragma once

#include <vector>
#include "ModulatedAllpass.h"
#include "RandomBuffer.h"

class AllpassDiffuser
{
public:
	static const int MaxStageCount = 8;

private:
	int samplerate;

	ModulatedAllpass filters[MaxStageCount];
	int delay;
	float modRate;
	std::vector<float> seedValues;
	int seed;
	float crossSeed;
		
public:
	int Stages;

	AllpassDiffuser()
	{
		crossSeed = 0.0;
		seed = 23456;
		UpdateSeeds();
		Stages = 1;

		SetSamplerate(48000);
	}

	int GetSamplerate()
	{
		return samplerate;
	}

	void SetSamplerate(int samplerate)
	{
		this->samplerate = samplerate;
		SetModRate(modRate);
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


	bool GetModulationEnabled()
	{
		return filters[0].ModulationEnabled;
	}

	void SetModulationEnabled(bool value)
	{
		for (int i = 0; i < MaxStageCount; i++)
			filters[i].ModulationEnabled = value;
			
	}

	void SetInterpolationEnabled(bool enabled)
	{
		for (int i = 0; i < MaxStageCount; i++)
			filters[i].InterpolationEnabled = enabled;
	}

	float* GetOutput()
	{
		return filters[Stages - 1].GetOutput();
	}

		
	void SetDelay(int delaySamples)
	{
		delay = delaySamples;
		Update();
	}

	void SetFeedback(float feedback)
	{
		for (int i = 0; i < MaxStageCount; i++)
			filters[i].Feedback = feedback;
	}

	void SetModAmount(float amount)
	{
		for (int i = 0; i < MaxStageCount; i++)
			filters[i].ModAmount = amount * (0.85 + 0.3 * seedValues[MaxStageCount + i]);
	}

	void SetModRate(float rate)
	{
		modRate = rate;

		for (int i = 0; i < MaxStageCount; i++)
			filters[i].ModRate = rate * (0.85 + 0.3 * seedValues[MaxStageCount * 2 + i]) / samplerate;
	}

	void Process(float* input, int sampleCount)
	{
		filters[0].Process(input, sampleCount);

		for (int i = 1; i < Stages; i++)
		{
			filters[i].Process(filters[i - 1].GetOutput(), sampleCount);
		}
	}

	void ClearBuffers()
	{
		for (int i = 0; i < MaxStageCount; i++)
			filters[i].ClearBuffers();
	}

private:
	void Update()
	{
		for (int i = 0; i < MaxStageCount; i++)
		{
			auto r = seedValues[i];
			auto d = std::pow(10, r) * 0.1; // 0.1 ... 1.0
			filters[i].SampleDelay = (int)(delay * d);
		}
	}

	void UpdateSeeds()
	{
		this->seedValues = RandomBuffer::Generate(seed, MaxStageCount * 3, crossSeed);
		Update();
	}

};
