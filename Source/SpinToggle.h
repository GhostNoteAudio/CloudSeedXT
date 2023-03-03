#pragma once

#include <JuceHeader.h>
#include "LayoutUtils.h"

class SpinToggleButton: public juce::ToggleButton
{
    juce::String getLabel()
    {
        auto toggled = getToggleState();
        return juce::String(toggled ? OnValue : OffValue);
    }

public:
    juce::String OffValue;
    juce::String OnValue;

    inline SpinToggleButton() : juce::ToggleButton("-")
    {
        OffValue = "Off";
        OnValue = "On";
    }

    inline ~SpinToggleButton() override
    {
    }

    inline void paintButton(juce::Graphics& g, bool highlighted, bool down) override
    {
        auto colour = findColour(juce::Slider::trackColourId);
        double scale = getHeight() / 24.0;
        g.setColour(colour);
        g.fillRoundedRectangle(1, 1, getWidth() - 2, getHeight() - 2, 4);

        colour = findColour(juce::Slider::thumbColourId);
        g.setColour(colour);
        g.setFont(getFontRegular(20 * scale));
        g.drawText(getLabel(), 2, 0, getWidth(), getHeight(), juce::Justification::centred);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpinToggleButton)
};