/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Authentication.h"

juce::String Authentication::AuthFileName = "CloudSeedXT.auth";
juce::String Authentication::AuthSalt = "";
bool Authentication::isAuthenticatedCacheSet = false;
bool Authentication::isAuthenticatedCacheValue = false;
bool Authentication::isFreePlugin = true;

template<typename T>
inline void Copy(T* dest, const T* source, int len)
{
    memcpy(dest, source, len * sizeof(T));
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(juce::AudioProcessorParameter::Listener* listener)
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    juce::NormalisableRange<float> range = { 0.0f, 1.0f, 0.0001f };

    for (int i = 0; i < Parameter::COUNT; i++)
    {    
        auto para = std::make_unique<juce::AudioParameterFloat>
        (
            ParameterIds[i], // parameterID
            ParameterLabel[i], // parameter name
            range, // min, max, delta
            0.0, // default value
            ParameterSuffix[i], // label
            juce::AudioProcessorParameter::genericParameter,
            [i](float value, int maxlen) { return FormatParameter(value, maxlen, i); } // formatter
        );
        para->addListener(listener);
        layout.add(std::move(para));
    }

    return layout;
}


CloudSeedXTAudioProcessor::CloudSeedXTAudioProcessor() : 
    AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, juce::Identifier("CloudSeedXT"), createParameterLayout(this))
{
    presetName = "Default Preset";

    for (int i = 0; i < Parameter::COUNT; i++)
    {
        parameterValueChanged(i, getParamByIdx(i));
    }
}

CloudSeedXTAudioProcessor::~CloudSeedXTAudioProcessor()
{
}

const juce::String CloudSeedXTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CloudSeedXTAudioProcessor::acceptsMidi() const
{
    return false;
}

bool CloudSeedXTAudioProcessor::producesMidi() const
{
    return false;
}

bool CloudSeedXTAudioProcessor::isMidiEffect() const
{
    return false;
}

double CloudSeedXTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CloudSeedXTAudioProcessor::getNumPrograms()
{
    return 1;
}

int CloudSeedXTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CloudSeedXTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CloudSeedXTAudioProcessor::getProgramName (int index)
{
    return {};
}

void CloudSeedXTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void CloudSeedXTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //if (expanderL.GetSamplerate() != sampleRate)
    //{
        //expanderL.SetSamplerate(sampleRate);
        //expanderR.SetSamplerate(sampleRate);
    //}
}

void CloudSeedXTAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool CloudSeedXTAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    auto numInsMain = layouts.getNumChannels(true, 0);
    auto numInsAux = layouts.getNumChannels(true, 1);
    auto numOuts = layouts.getNumChannels(false, 0);
    
    if (numInsMain == 1 && numInsAux == 1 && numOuts == 1) // mono processor
        return true;
    if (numInsMain == 2 && (numInsAux == 1 || numInsAux == 2) && numOuts == 2) // stereo processor, aux can be mono
        return true;
    return false;
}

void CloudSeedXTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //bool stereo = getParamByIdx((int)Parameters::Stereo) >= 0.5;
    //bool aux = getParamByIdx((int)Parameters::Aux) >= 0.5;

    //juce::ScopedNoDenormals noDenormals;
    //int numCh = buffer.getNumChannels(); // todo use this!!!
    //int len = buffer.getNumSamples();
    //auto totalIns = getTotalNumInputChannels();
    //auto totalOuts = getTotalNumOutputChannels();

    //if (totalIns == 0 || totalOuts == 0)
    //    return;
    //if (numCh < totalIns)
    //    totalIns = numCh;
    //if (numCh < totalOuts)
    //    totalOuts = numCh;

    //// Sensor is always the left channel, no matter whether running stereo mode or Aux mode or both.
    //int auxIdx = (aux && totalIns >= 3) ? 2 : 0;

    //const float* inputL = buffer.getReadPointer(0);
    //const float* inputR = buffer.getReadPointer(totalIns > 1 ? 1 : 0);
    //const float* sensor = buffer.getReadPointer(auxIdx);

    //float* outputL = buffer.getWritePointer(0);
    //float* outputR = buffer.getWritePointer(totalOuts > 1 ? 1 : 0);

    //Copy(bufL, inputL, len);
    //Copy(bufR, inputR, len);
    //Copy(bufS, sensor, len);

    ////expanderL.Process(bufL, bufS, len);
    ////expanderR.Process(bufR, bufS, len);

    //trialMode.Process(bufL, len, false);
    //trialMode.Process(bufR, len, true);

    //if (stereo)
    //{
    //    Copy(outputL, bufL, len);
    //    if (totalOuts > 1)
    //        Copy(outputR, bufR, len);
    //}
    //else
    //{
    //    Copy(outputL, bufL, len);
    //    if (totalOuts > 1)
    //        Copy(outputR, bufL, len);
    //}
}

//==============================================================================
bool CloudSeedXTAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CloudSeedXTAudioProcessor::createEditor()
{ 
    return new CloudSeedXTAudioProcessorEditor(*this);
}

//==============================================================================
void CloudSeedXTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void CloudSeedXTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

void CloudSeedXTAudioProcessor::parameterValueChanged(int idx, float value)
{
    auto scaledVal = ScaleParam(value, idx);

    //switch(idx)
    //{
    //case (int)Parameters::BandUpper:
    //    expanderL.BandUpper = scaledVal;
    //    expanderR.BandUpper = scaledVal;
    //    expanderL.ResetBands();
    //    expanderR.ResetBands();
    //    break;
    //case (int)Parameters::BandGap:
    //    expanderL.BandGap = scaledVal;
    //    expanderR.BandGap = scaledVal;
    //    expanderL.ResetBands();
    //    expanderR.ResetBands();
    //    break;
    //case (int)Parameters::Expansion:
    //    expanderL.Expansion = scaledVal;
    //    expanderR.Expansion = scaledVal;
    //    break;
    //case (int)Parameters::DecayMs:
    //    expanderL.DecayMs = scaledVal;
    //    expanderR.DecayMs = scaledVal;
    //    break;
    //case (int)Parameters::Knee:
    //    expanderL.Knee = scaledVal;
    //    expanderR.Knee = scaledVal;
    //    break;
    //case (int)Parameters::Hysteresis:
    //    expanderL.Hysteresis = scaledVal;
    //    expanderR.Hysteresis = scaledVal;
    //    expanderL.ResetBands();
    //    expanderR.ResetBands();
    //    break;
    //}
}

void CloudSeedXTAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{

}

float CloudSeedXTAudioProcessor::getParamByIdx(int idx)
{
    return parameters.getParameter(ParameterIds[idx])->getValue();
}

float CloudSeedXTAudioProcessor::getScaledParamByIdx(int idx)
{
    return ScaleParam(parameters.getParameter(ParameterIds[idx])->getValue(), idx);
}

void CloudSeedXTAudioProcessor::setPresetName(juce::String presetName)
{
    this->presetName = presetName;
}

juce::String CloudSeedXTAudioProcessor::getPresetName()
{
    return this->presetName;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CloudSeedXTAudioProcessor();
}
