#pragma once

#include <JuceHeader.h>
#include "Spinner.h"

static juce::Colour ColourTitle = juce::Colour::fromRGB(50, 102, 196 );
static juce::Colour ColourLightBlue = juce::Colour::fromRGB(103, 147, 222);
static juce::Colour ColourTrack = juce::Colour::fromFloatRGBA(0, 0, 0, 0.24);
static juce::Colour ColourPrimary = juce::Colour::fromRGB(62, 132, 249);
static juce::Colour ColourKnobLabel = juce::Colour::fromRGBA(150, 177, 230, 255);

inline void styleSpinnerSeed(Spinner* spinner)
{
	spinner->setMouseDragSensitivity(1000);
	spinner->setColour(juce::Slider::trackColourId, juce::Colour::fromFloatRGBA(0, 0, 0, 0));
	spinner->setColour(juce::Slider::thumbColourId, ColourLightBlue);
}

inline void styleKnobLabel(juce::Label* label)
{
	label->setJustificationType(juce::Justification::centred);
	label->setColour(juce::Label::ColourIds::textColourId, ColourKnobLabel);
	label->setFont(getFontLight(26));
}
