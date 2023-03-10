#pragma once

#include "Lp1.h"
#include "ModulatedDelay.h"
#include "AllpassDiffuser.h"
#include "Biquad.h"

namespace Cloudseed
{
	class DelayLine
	{
	private:
		ModulatedDelay delay;
		AllpassDiffuser diffuser;
		Biquad lowShelf;
		Biquad highShelf;
		Lp1 lowPass;
		float filterOutputBuffer[BUFFER_SIZE] = { 0 };
		float feedback;

	public:
		bool DiffuserEnabled;
		bool LowShelfEnabled;
		bool HighShelfEnabled;
		bool CutoffEnabled;
		bool TapPostDiffuser;

		DelayLine() :
			lowShelf(Biquad::FilterType::LowShelf, 48000),
			highShelf(Biquad::FilterType::HighShelf, 48000)
		{
			feedback = 0;

			lowShelf.SetGainDb(-20);
			lowShelf.Frequency = 20;

			highShelf.SetGainDb(-20);
			highShelf.Frequency = 19000;

			lowPass.SetCutoffHz(1000);
			lowShelf.Update();
			highShelf.Update();
			SetSamplerate(48000);
			SetDiffuserSeed(1, 0.0);
		}

		void SetSamplerate(int samplerate)
		{
			diffuser.SetSamplerate(samplerate);
			lowPass.SetSamplerate(samplerate);
			lowShelf.SetSamplerate(samplerate);
			highShelf.SetSamplerate(samplerate);
		}

		void SetDiffuserSeed(int seed, float crossSeed)
		{
			diffuser.SetSeed(seed);
			diffuser.SetCrossSeed(crossSeed);
		}

		void SetDelay(int delaySamples)
		{
			delay.SampleDelay = delaySamples;
		}

		void SetFeedback(float feedb)
		{
			feedback = feedb;
		}

		void SetDiffuserDelay(int delaySamples)
		{
			diffuser.SetDelay(delaySamples);
		}

		void SetDiffuserFeedback(float feedb)
		{
			diffuser.SetFeedback(feedb);
		}

		void SetDiffuserStages(int stages)
		{
			diffuser.Stages = stages;
		}

		void SetLowShelfGain(float gainDb)
		{
			lowShelf.SetGainDb(gainDb);
			lowShelf.Update();
		}

		void SetLowShelfFrequency(float frequency)
		{
			lowShelf.Frequency = frequency;
			lowShelf.Update();
		}

		void SetHighShelfGain(float gainDb)
		{
			highShelf.SetGainDb(gainDb);
			highShelf.Update();
		}

		void SetHighShelfFrequency(float frequency)
		{
			highShelf.Frequency = frequency;
			highShelf.Update();
		}

		void SetCutoffFrequency(float frequency)
		{
			lowPass.SetCutoffHz(frequency);
		}

		void SetLineModAmount(float amount)
		{
			delay.ModAmount = amount;
		}

		void SetLineModRate(float rate)
		{
			delay.ModRate = rate;
		}

		void SetDiffuserModAmount(float amount)
		{
			diffuser.SetModulationEnabled(amount > 0.0);
			diffuser.SetModAmount(amount);
		}

		void SetDiffuserModRate(float rate)
		{
			diffuser.SetModRate(rate);
		}

		void SetInterpolationEnabled(bool value)
		{
			diffuser.SetInterpolationEnabled(value);
		}

		void Process(float* input, float* output, int bufSize)
		{
			float tempBuffer[BUFFER_SIZE];

			for (int i = 0; i < bufSize; i++)
				tempBuffer[i] = input[i] + filterOutputBuffer[i] * feedback;

			delay.Process(tempBuffer, tempBuffer, bufSize);
			
			if (!TapPostDiffuser)
				Utils::Copy(output, tempBuffer, bufSize);
			if (DiffuserEnabled)
				diffuser.Process(tempBuffer, tempBuffer, bufSize);
			if (LowShelfEnabled)
				lowShelf.Process(tempBuffer, tempBuffer, bufSize);
			if (HighShelfEnabled)
				highShelf.Process(tempBuffer, tempBuffer, bufSize);
			if (CutoffEnabled)
				lowPass.Process(tempBuffer, tempBuffer, bufSize);

			Utils::Copy(filterOutputBuffer, tempBuffer, bufSize);

			if (TapPostDiffuser)
				Utils::Copy(output, tempBuffer, bufSize);
		}

		void ClearDiffuserBuffer()
		{
			diffuser.ClearBuffers();
		}

		void ClearBuffers()
		{
			delay.ClearBuffers();
			diffuser.ClearBuffers();
			lowShelf.ClearBuffers();
			highShelf.ClearBuffers();
			lowPass.Output = 0;
			Utils::ZeroBuffer(filterOutputBuffer, BUFFER_SIZE);
		}
	};
}
