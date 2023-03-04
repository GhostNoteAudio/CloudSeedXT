#pragma once

#include <vector>
#include "../Parameters.h"
#include "ReverbChannel.h"
#include "AllpassDiffuser.h"
#include "MultitapDelay.h"
#include "Utils.h"

namespace Cloudseed
{
	class ReverbController
	{
	private:
		int samplerate;

		ReverbChannel channelL;
		ReverbChannel channelR;
		double parameters[(int)Parameter::COUNT];

	public:
		ReverbController(int samplerate) :
			channelL(samplerate, ChannelLR::Left),
			channelR(samplerate, ChannelLR::Right)
		{
			this->samplerate = samplerate;
		}

		int GetSamplerate()
		{
			return samplerate;
		}

		void SetSamplerate(int samplerate)
		{
			this->samplerate = samplerate;
			channelL.SetSamplerate(samplerate);
			channelR.SetSamplerate(samplerate);
		}

		int GetParameterCount()
		{
			return Parameter::COUNT;
		}

		double* GetAllParameters()
		{
			return parameters;
		}

		void SetParameter(int paramId, double value)
		{
			parameters[paramId] = value;
			auto scaled = ScaleParam(value, paramId);
			channelL.SetParameter(paramId, scaled);
			channelR.SetParameter(paramId, scaled);
		}

		void ClearBuffers()
		{
			channelL.ClearBuffers();
			channelR.ClearBuffers();
		}

		void Process(float* inL, float* inR, float* outL, float* outR, int bufSize)
		{
			while (bufSize > 0)
			{
				int subBufSize = bufSize > 64 ? 64 : bufSize;
				ProcessChunk(inL, inR, outL, outR, subBufSize);
				inL = &inL[subBufSize];
				inR = &inR[subBufSize];
				outL = &outL[subBufSize];
				outR = &outR[subBufSize];
				bufSize -= subBufSize;
			}
		}

	private:
		void ProcessChunk(float* inL, float* inR, float* outL, float* outR, int bufSize)
		{
			float leftChannelIn[BUFFER_SIZE];
			float rightChannelIn[BUFFER_SIZE];

			float inputMix = ScaleParam(parameters[Parameter::InputMix], Parameter::InputMix);
			float cm = inputMix * 0.5;
			float cmi = (1 - cm);

			for (int i = 0; i < bufSize; i++)
			{
				leftChannelIn[i] = inL[i] * cmi + inR[i] * cm;
				rightChannelIn[i] = inR[i] * cmi + inL[i] * cm;
			}

			channelL.Process(leftChannelIn, outL, bufSize);
			channelR.Process(rightChannelIn, outR, bufSize);
		}
	};
}
