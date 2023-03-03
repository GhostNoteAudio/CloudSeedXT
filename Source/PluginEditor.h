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

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
const int ResizeTabSize = 24;

//class StereoButton : public juce::ToggleButton
//{
//public:
//    inline StereoButton() : juce::ToggleButton("Stereo")
//    {
//
//    }
//
//    inline ~StereoButton() override
//    {
//    }
//
//    inline void paintButton(juce::Graphics& g, bool highlighted, bool down) override
//    {
//        auto toggled = getToggleState();
//        juce::Path p;
//        auto value = toggled ? 0.6f : 0.3f;
//        value = value + highlighted * 0.15;
//        g.beginTransparencyLayer(value);
//        g.setColour(juce::Colour::fromFloatRGBA(0, 0, 0, 1));
//        g.fillEllipse(getHeight() / 2 + 1, 1, getHeight() - 2, getHeight() - 2);
//        g.drawEllipse(1.75, 1.75, getHeight() - 3.5, getHeight() - 3.5, 1.5);
//        g.endTransparencyLayer();
//    }
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoButton)
//};

//class AuxButton : public juce::ToggleButton
//{
//public:
//    inline AuxButton() : juce::ToggleButton("AUX")
//    {
//
//    }
//
//    inline ~AuxButton() override
//    {
//    }
//
//    inline void paintButton(juce::Graphics& g, bool highlighted, bool down) override
//    {
//        auto toggled = getToggleState();
//        juce::Path p;
//        auto value = toggled ? 0.6f : 0.3f;
//        value = value + highlighted * 0.15;
//        g.beginTransparencyLayer(value);
//        g.setColour(juce::Colour::fromFloatRGBA(0, 0, 0, 1));
//        g.setFont(getFontSemibold(22));
//        g.drawText("AUX", 1, 1, getWidth() - 2, getHeight() - 2, false);
//        g.endTransparencyLayer();
//    }
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AuxButton)
//};

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
        if (event.eventComponent != this)
            return;

        if (event.x < 320 && event.y <= 100)
        {
            overlay.setVisible(true);
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
    
    juce::Label pluginName;
    juce::Label demoStatus;
    juce::Label parameterReadout;
    ModalOverlay overlay;
    AboutDialog aboutDialog;
    bool scaleBig = false;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CloudSeedXTAudioProcessorEditor)
};
