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
    parameters(*this, nullptr, juce::Identifier("CloudSeedXT"), createParameterLayout(this)),
    reverb(48000)
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

void CloudSeedXTAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String CloudSeedXTAudioProcessor::getProgramName(int index)
{
    return presetName;
}

void CloudSeedXTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void CloudSeedXTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if (reverb.GetSamplerate() != sampleRate)
        reverb.SetSamplerate(sampleRate);
}

void CloudSeedXTAudioProcessor::releaseResources()
{
    
}

bool CloudSeedXTAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    auto numInsMain = layouts.getNumChannels(true, 0);
    auto numOuts = layouts.getNumChannels(false, 0);
    return numInsMain == 2 && numOuts == 2;
}

void CloudSeedXTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    int bufSize = buffer.getNumSamples();
    float* inputL = (float*)buffer.getReadPointer(0);
    float* inputR = (float*)buffer.getReadPointer(1);
    float* outputL = buffer.getWritePointer(0);
    float* outputR = buffer.getWritePointer(1);
    reverb.Process(inputL, inputR, outputL, outputR, bufSize);
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
    reverb.SetParameter(idx, value);
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
