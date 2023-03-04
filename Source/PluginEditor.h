#pragma once

#include <JuceHeader.h>
#include <functional>
#include "PluginProcessor.h"
#include "RotaryKnobLive.h"
#include "Meter.h"
#include "LayoutUtils.h"
#include "AboutDialog.h"
#include "Spinner.h"
#include "Stylers.h"
#include "SpinToggle.h"
#include "Presets.h"
#include "InputDialog.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
const int ResizeTabSize = 24;

class CloudSeedXTAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    CloudSeedXTAudioProcessorEditor (CloudSeedXTAudioProcessor&);
    ~CloudSeedXTAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void visibilityChanged() override { applyEditorScaling(); }
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseEnter(const juce::MouseEvent& ev) override;
    void mouseExit(const juce::MouseEvent& ev) override;
    void timerCallback() override;

private:
    void attachParam(juce::Slider* component, int parameter);
    void attachParam(juce::ToggleButton* component, int parameter);
    void addKnobLabels();
    void showAboutDialog();
    void reloadPresetName();
    void savePreset();
    void applyEditorScaling();

    CloudSeedXTAudioProcessor& audioProcessor;
    int currentParameter;
    juce::CriticalSection objectLock;
    std::map<juce::Component*, int> componentToParam;
    
    Spinner spinnersSeed[4];
    RotaryKnobLive knobs[28];
    juce::Label knobLabels[28];
    Spinner spinnersParams[3];
    SpinToggleButton spinToggleButtons[10];

    std::unique_ptr<SliderAttachment> attachments[Parameter::COUNT];
    std::unique_ptr<ButtonAttachment> buttonAttachments[Parameter::COUNT];
    
    InputDialog dialog;
    juce::Label pluginName;
    juce::Label parameterReadout;
    juce::Label presetName;
    ModalOverlay overlay;
    AboutDialog aboutDialog;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudSeedXTAudioProcessorEditor)
};
