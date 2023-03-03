#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotaryKnobLive.h"
#include "Meter.h"
#include "LayoutUtils.h"
#include "AboutDialog.h"
#include "Spinner.h"
#include "Stylers.h"
#include <functional>
#include "SpinToggle.h"
#include "Presets.h"
#include "InputDialog.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
const int ResizeTabSize = 24;

class CloudSeedXTAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer, public juce::MouseListener
{
public:
    CloudSeedXTAudioProcessorEditor (CloudSeedXTAudioProcessor&);
    ~CloudSeedXTAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& ev) override;
    void mouseExit(const juce::MouseEvent& ev) override;

    void addKnobLabels();
    void attachParam(juce::Slider* component, int parameter);
    void attachParam(juce::ToggleButton* component, int parameter);

    inline void mouseMove(const juce::MouseEvent& event) override
    {
        if (event.eventComponent != this)
            return;

        if (event.x < 320 && event.y <= 100)
            setMouseCursor(juce::MouseCursor::PointingHandCursor);
        else if (event.x + event.y >= getWidth() + getHeight() - ResizeTabSize)
            setMouseCursor(juce::MouseCursor::PointingHandCursor);
        else
            setMouseCursor(juce::MouseCursor::NormalCursor);
    }

    inline void mouseDown(const juce::MouseEvent& event) override
    {
        if (event.eventComponent == &presetName)
        {
            Presets::showPresetMenu(
                &this->audioProcessor, 
                [this]() {this->reloadPresetName(); }, 
                [this]() {this->savePreset(); }
            );
        }

        if (event.eventComponent != this)
            return;

        if (event.x < 320 && event.y <= 100)
        {
            showAboutDialog();
        }

        if (event.x + event.y >= getWidth() + getHeight() - ResizeTabSize)
            scaleBig = !scaleBig;

        if (scaleBig)
            setScaleFactor(1.0);
        else
            setScaleFactor(0.75);

        repaint();
    }

    virtual void timerCallback() override;

private:

    void showAboutDialog();
    void reloadPresetName();
    void savePreset();

    CloudSeedXTAudioProcessor& audioProcessor;
    int currentParameter;
    juce::CriticalSection objectLock;
    std::map<juce::Component*, int> componentToParam;
    
    Spinner spinnersSeed[4];
    RotaryKnobLive knobs[28];
    juce::Label knobLabels[28];

    Spinner spinnersParams[3];
    SpinToggleButton spinToggleButtons[9];

    std::unique_ptr<SliderAttachment> attachments[Parameter::COUNT];
    std::unique_ptr<ButtonAttachment> buttonAttachments[Parameter::COUNT];
    
    InputDialog dialog;
    juce::Label pluginName;
    juce::Label parameterReadout;
    juce::Label presetName;
    ModalOverlay overlay;
    AboutDialog aboutDialog;
    bool scaleBig = true;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudSeedXTAudioProcessorEditor)
};
