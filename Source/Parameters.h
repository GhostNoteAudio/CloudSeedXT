#pragma once

#include <JuceHeader.h>
#include "DSP/Utils.h"

namespace Parameter
{
    const int Interpolation = 0;
    const int LowCutEnabled = 1;
    const int HighCutEnabled = 2;
    const int InputMix = 3;
    const int LowCut = 4;
    const int HighCut = 5;
    const int DryOut = 6;
    const int EarlyOut = 7;
    const int LateOut = 8;

    const int TapEnabled = 9;
    const int TapCount = 10;
    const int TapDecay = 11;
    const int TapPredelay = 12;
    const int TapLength = 13;

    const int EarlyDiffuseEnabled = 14;
    const int EarlyDiffuseCount = 15;
    const int EarlyDiffuseDelay = 16;
    const int EarlyDiffuseModAmount = 17;
    const int EarlyDiffuseFeedback = 18;
    const int EarlyDiffuseModRate = 19;

    const int LateMode = 20;
    const int LateLineCount = 21;
    const int LateDiffuseEnabled = 22;
    const int LateDiffuseCount = 23;
    const int LateLineSize = 24;
    const int LateLineModAmount = 25;
    const int LateDiffuseDelay = 26;
    const int LateDiffuseModAmount = 27;
    const int LateLineDecay = 28;
    const int LateLineModRate = 29;
    const int LateDiffuseFeedback = 30;
    const int LateDiffuseModRate = 31;

    const int EqLowShelfEnabled = 32;
    const int EqHighShelfEnabled = 33;
    const int EqLowpassEnabled = 34;
    const int EqLowFreq = 35;
    const int EqHighFreq = 36;
    const int EqCutoff = 37;
    const int EqLowGain = 38;
    const int EqHighGain = 39;
    const int EqCrossSeed = 40;

    const int SeedTap = 41;
    const int SeedDiffusion = 42;
    const int SeedDelay = 43;
    const int SeedPostDiffusion = 44;

    const int COUNT = 45;
};

extern const char* ParameterIds[Parameter::COUNT];
extern const char* ParameterLabel[Parameter::COUNT];

inline double ScaleParam(double val, int index)
{
    switch (index)
    {
    case Parameter::Interpolation:
    case Parameter::LowCutEnabled:
    case Parameter::HighCutEnabled:
    case Parameter::TapEnabled:
    case Parameter::LateDiffuseEnabled:
    case Parameter::EqLowShelfEnabled:
    case Parameter::EqHighShelfEnabled:
    case Parameter::EqLowpassEnabled:
    case Parameter::EarlyDiffuseEnabled:
        return val < 0.5 ? 0.0 : 1.0;

    case Parameter::InputMix:
    case Parameter::EarlyDiffuseFeedback:
    case Parameter::TapDecay:
    case Parameter::LateDiffuseFeedback:
    case Parameter::EqCrossSeed:
        return val;

    case Parameter::SeedTap:
    case Parameter::SeedDiffusion:
    case Parameter::SeedDelay:
    case Parameter::SeedPostDiffusion:
        return (int)std::floor(val * 999.999);

    case Parameter::LowCut:
        return 20 + Utils::Resp4oct(val) * 980;
    case Parameter::HighCut:
        return 400 + Utils::Resp4oct(val) * 19600;
    
    case Parameter::DryOut:
    case Parameter::EarlyOut:
    case Parameter::LateOut:
        return -30 + val * 30;
    
    case Parameter::TapCount:
        return (int)(1 + val * 255);
    case Parameter::TapPredelay:
        return Utils::Resp1dec(val) * 500;
    case Parameter::TapLength:
        return 10 + val * 990;
    
    case Parameter::EarlyDiffuseCount:
        return (int)(1 + val * 11.999);
    case Parameter::EarlyDiffuseDelay:
        return 10 + val * 90;
    case Parameter::EarlyDiffuseModAmount:
        return val * 2.5;
    case Parameter::EarlyDiffuseModRate:
        return Utils::Resp2dec(val) * 5;

    case Parameter::LateMode:
        return val < 0.5 ? 0.0 : 1.0;
    case Parameter::LateLineCount:
        return (int)(1 + val * 11.999);
    case Parameter::LateDiffuseCount:
        return (int)(1 + val * 7.999);
    case Parameter::LateLineSize:
        return 20 + Utils::Resp2dec(val) * 980; 
    case Parameter::LateLineModAmount:
        return val * 2.5;
    case Parameter::LateDiffuseDelay:
        return 10 + val * 90;
    case Parameter::LateDiffuseModAmount:
        return val * 2.5;
    case Parameter::LateLineDecay:
        return 0.05 + Utils::Resp3dec(val) * 59.95;
    case Parameter::LateLineModRate:
        return Utils::Resp2dec(val) * 5;
    case Parameter::LateDiffuseModRate:
        return Utils::Resp2dec(val) * 5;

    case Parameter::EqLowFreq:
        return 20 + Utils::Resp3oct(val) * 980;
    case Parameter::EqHighFreq:
        return 400 + Utils::Resp4oct(val) * 19600;
    case Parameter::EqCutoff:
        return 400 + Utils::Resp4oct(val) * 19600;
    case Parameter::EqLowGain:
        return -20 + val * 20;
    case Parameter::EqHighGain:
        return -20 + val * 20;
    }
    return 0;
}

inline juce::String FormatParameter(float val, int maxLen, int paramId)
{
    double s = ScaleParam(val, paramId);

    switch (paramId)
    {
    case Parameter::Interpolation:
    case Parameter::HighCutEnabled:
    case Parameter::LowCutEnabled:
    case Parameter::TapEnabled:
    case Parameter::LateDiffuseEnabled:
    case Parameter::EqLowShelfEnabled:
    case Parameter::EqHighShelfEnabled:
    case Parameter::EqLowpassEnabled:
    case Parameter::EarlyDiffuseEnabled:
        return ScaleParam(val, paramId) == 1 ? "Enabled" : "Disabled";

    case Parameter::InputMix:
    case Parameter::EarlyDiffuseFeedback:
    case Parameter::TapDecay:
    case Parameter::LateDiffuseFeedback:
    case Parameter::EqCrossSeed:
        return juce::String((int)(s * 100)) + "%";

    case Parameter::SeedTap:
    case Parameter::SeedDiffusion:
    case Parameter::SeedDelay:
    case Parameter::SeedPostDiffusion:
        char tmp[32];
        sprintf(tmp, "%03d", (int)s);
        return juce::String(tmp);

    case Parameter::LowCut:
    case Parameter::HighCut:
    case Parameter::EqLowFreq:
    case Parameter::EqHighFreq:
    case Parameter::EqCutoff:
        return juce::String((int)s) + " Hz";

    case Parameter::DryOut:
    case Parameter::EarlyOut:
    case Parameter::LateOut:
        return s <= -30 ? "Muted" : juce::String(s, 1) + " dB";

    case Parameter::TapCount:
    case Parameter::EarlyDiffuseCount:
    case Parameter::LateLineCount:
    case Parameter::LateDiffuseCount:
        return juce::String((int)s);

    case Parameter::TapPredelay:
    case Parameter::TapLength:
    case Parameter::EarlyDiffuseDelay:
    case Parameter::LateLineSize:
    case Parameter::LateDiffuseDelay:
        return juce::String((int)s) + " ms";

    case Parameter::LateLineDecay:
        if (s < 1)
            return juce::String((int)(s * 1000)) + " ms";
        else if (s < 10)
            return juce::String(s, 2) + " sec";
        else
            return juce::String(s, 1) + " sec";

    case Parameter::LateMode:
        return s == 1 ? "Post" : "Pre";

    case Parameter::EarlyDiffuseModAmount:
    case Parameter::LateLineModAmount:
    case Parameter::LateDiffuseModAmount:
        return juce::String((int)(val * 100)) + " %";
    
    case Parameter::EarlyDiffuseModRate:
    case Parameter::LateLineModRate:
    case Parameter::LateDiffuseModRate:
        return juce::String(s, 2) + " Hz";

    case Parameter::EqLowGain:
    case Parameter::EqHighGain:
        return juce::String(s, 1) + " dB";

    default:
        return juce::String(s, 2);
    }
}
