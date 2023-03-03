/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LayoutUtils.h"

const int PanelPadding = 15;
const int PanelSpacing = 15;
const int KnobSpacing = 100;

const int Panel0x = 20;
const int Panel0Width = 2 * PanelPadding + 3 * KnobSpacing;
const int Panel1x = Panel0x + Panel0Width + PanelSpacing;
const int Panel1Width = 2 * PanelPadding + 2 * KnobSpacing;
const int Panel2x = Panel1x + Panel1Width + PanelSpacing;
const int Panel2Width = 2 * PanelPadding + 2 * KnobSpacing;
const int Panel3x = Panel2x + Panel2Width + PanelSpacing;
const int Panel3Width = 2 * PanelPadding + 4 * KnobSpacing;
const int Panel4x = Panel3x + Panel3Width + PanelSpacing;
const int Panel4Width = 2 * PanelPadding + 3 * KnobSpacing;
const int PluginWidth = Panel4x + Panel4Width + 20;

//==============================================================================
CloudSeedXTAudioProcessorEditor::CloudSeedXTAudioProcessorEditor (CloudSeedXTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    const juce::ScopedLock myScopedLock(objectLock);
    currentParameter = Parameter::COUNT;

    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromHSL(0, 0, 0, 0.0));
    getLookAndFeel().setColour(juce::Slider::trackColourId, ColourTrack);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, ColourPrimary);
    getLookAndFeel().setDefaultSansSerifTypeface(getTypefaceSemibold());

    parameterReadout.setFont(getFontLight(36));
    parameterReadout.setText(" ", juce::NotificationType::dontSendNotification);
    parameterReadout.setJustificationType(juce::Justification::centredRight);
    parameterReadout.setColour(juce::Label::ColourIds::textColourId, ColourLightBlue);
    parameterReadout.setBounds(PluginWidth - 420, 635, 400, 32);
    addAndMakeVisible(parameterReadout);

    for (int i = 0; i < 4; i++)
    {
        auto param = Parameter::SeedTap + i;
        spinnersSeed[i].setBounds(10 + i * 90, 630, 90, 40);
        styleSpinnerSeed(&spinnersSeed[i]);
        spinnersSeed[i].Formatter = [param](double val) { return FormatParameter(val, 32, param); };
        addAndMakeVisible(spinnersSeed[i]);
        attachParam(&spinnersSeed[i], param);
    }

    int y0 = 296;
    int y1 = 430;
    int w = KnobSpacing;
    int h = 70;
    int i = 0;

    int x0 = Panel0x + PanelPadding;
    i = 0;  knobs[i].setBounds(x0 + 0*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::InputMix);
    i = 1;  knobs[i].setBounds(x0 + 1*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LowCut);
    i = 2;  knobs[i].setBounds(x0 + 2*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::HighCut);
    i = 3;  knobs[i].setBounds(x0 + 0*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::DryOut);
    i = 4;  knobs[i].setBounds(x0 + 1*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EarlyOut);
    i = 5;  knobs[i].setBounds(x0 + 2*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateOut);

    x0 = Panel1x + PanelPadding;
    i = 6;  knobs[i].setBounds(x0 + 0*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::TapDensity);
    i = 7;  knobs[i].setBounds(x0 + 1*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::TapDecay);
    i = 8;  knobs[i].setBounds(x0 + 0*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::TapPredelay);
    i = 9;  knobs[i].setBounds(x0 + 1*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::TapLength);

    x0 = Panel2x + PanelPadding;
    i = 10; knobs[i].setBounds(x0 + 0*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EarlyDiffuseDelay);
    i = 11; knobs[i].setBounds(x0 + 1*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EarlyDiffuseModAmount);
    i = 12; knobs[i].setBounds(x0 + 0*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EarlyDiffuseFeedback);
    i = 13; knobs[i].setBounds(x0 + 1*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EarlyDiffuseModRate);

    x0 = Panel3x + PanelPadding;
    i = 14; knobs[i].setBounds(x0 + 0*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateLineSize);
    i = 15; knobs[i].setBounds(x0 + 1*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateLineModAmount);
    i = 16; knobs[i].setBounds(x0 + 2*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateDiffuseDelay);
    i = 17; knobs[i].setBounds(x0 + 3*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateDiffuseModAmount);
    i = 18; knobs[i].setBounds(x0 + 0*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateLineDecay);
    i = 19; knobs[i].setBounds(x0 + 1*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateLineModRate);
    i = 20; knobs[i].setBounds(x0 + 2*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateDiffuseFeedback);
    i = 21; knobs[i].setBounds(x0 + 3*w, y1, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::LateDiffuseModRate);
    
    x0 = Panel4x + PanelPadding;
    i = 22; knobs[i].setBounds(x0 + 0 * w, y0, w, h);   addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EqLowFreq);
    i = 23; knobs[i].setBounds(x0 + 1 * w, y0, w, h);   addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EqHighFreq);
    i = 24; knobs[i].setBounds(x0 + 2 * w, y0, w, h);   addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EqCutoff);
    i = 25; knobs[i].setBounds(x0 + 0 * w, y1, w, h);   addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EqLowGain);
    i = 26; knobs[i].setBounds(x0 + 1 * w, y1, w, h);   addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EqHighGain);
    i = 27; knobs[i].setBounds(x0 + 2 * w, y1, w, h);   addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::EqCrossSeed);

    addKnobLabels();

    int sw = w - 20; // spinner width
    int sh = 28; // spinner height
    int sy = 240; // spinner y pos

    spinnersParams[0].setBounds(Panel2x + PanelPadding + w + 10, sy, sw, sh);
    attachParam(&spinnersParams[0], Parameter::EarlyDiffuseCount);
    spinnersParams[1].setBounds(Panel3x + PanelPadding + w + 10, sy, sw, sh);
    attachParam(&spinnersParams[1], Parameter::LateLineCount);
    spinnersParams[2].setBounds(Panel3x + PanelPadding + 3*w + 10, sy, sw, sh);
    attachParam(&spinnersParams[2], Parameter::LateDiffuseCount);
    addAndMakeVisible(spinnersParams[0]);
    addAndMakeVisible(spinnersParams[1]);
    addAndMakeVisible(spinnersParams[2]);


    spinToggleButtons[0].setBounds(Panel0x + PanelPadding + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[0], Parameter::Interpolation);
    spinToggleButtons[1].setBounds(Panel0x + PanelPadding + w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[1], Parameter::HighCutEnabled);
    spinToggleButtons[2].setBounds(Panel0x + PanelPadding + 2*w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[2], Parameter::LowCutEnabled);

    spinToggleButtons[3].setBounds(Panel2x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[3], Parameter::EarlyDiffuseEnabled);

    spinToggleButtons[4].setBounds(Panel3x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[4], Parameter::LateMode);
    spinToggleButtons[4].OffValue = "Pre";
    spinToggleButtons[4].OnValue = "Post";
    spinToggleButtons[5].setBounds(Panel3x + PanelPadding + 2 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[5], Parameter::LateDiffuseEnabled);

    spinToggleButtons[6].setBounds(Panel4x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[6], Parameter::EqLowShelfEnabled);
    spinToggleButtons[7].setBounds(Panel4x + PanelPadding + 1 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[7], Parameter::EqHighShelfEnabled);
    spinToggleButtons[8].setBounds(Panel4x + PanelPadding + 2 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[8], Parameter::EqLowpassEnabled);

    for (int i = 0; i < 9; i++)
        addAndMakeVisible(spinToggleButtons[i]);

    setSize(PluginWidth, 700);

    pluginName.setText("Cloud Seed XT", juce::NotificationType::dontSendNotification);
    pluginName.setFont(getFontLight(70));
    pluginName.setJustificationType(juce::Justification::centredTop);
    pluginName.setColour(juce::Label::ColourIds::textColourId, ColourTitle);
    pluginName.setBounds(getWidth()/2 - 300, 10, 600, 70);
    addAndMakeVisible(pluginName);

    aboutDialog.setBounds(0, 0, 550, 180);
    aboutDialog.SetOverlay(&overlay);
    overlay.setComponent(&aboutDialog);
    overlay.setBounds(0, 0, getWidth(), getHeight());
    addAndMakeVisible(overlay);
    overlay.setVisible(false);
    
    startTimerHz(30);

    setScaleFactor(0.75);
    // Adjust the About dialog to compensate for small scaling factor
    overlay.setTransform(juce::AffineTransform::scale(1.5, 1.5, getWidth() / 2, getHeight() / 2));
}

void CloudSeedXTAudioProcessorEditor::addKnobLabels()
{
    for (int i = 0; i < 28; i++)
    {
        auto knob = &this->knobs[i];
        int param = componentToParam[knob];
        styleKnobLabel(&knobLabels[i]);
        knobLabels[i].setText(ParameterLabel[param], juce::NotificationType::dontSendNotification);
        knobLabels[i].setBounds(knob->getPosition().x - 10, knob->getPosition().y + 74, knob->getWidth() + 20, 30);
        addAndMakeVisible(knobLabels[i]);
    }
}

void CloudSeedXTAudioProcessorEditor::attachParam(juce::Slider* component, int parameter)
{
    attachments[parameter].reset(new SliderAttachment(audioProcessor.parameters, juce::String(ParameterIds[parameter]), *component));
    component->addMouseListener(this, false);
    componentToParam[component] = parameter;
}

void CloudSeedXTAudioProcessorEditor::attachParam(juce::ToggleButton* component, int parameter)
{
    buttonAttachments[parameter].reset(new ButtonAttachment(audioProcessor.parameters, juce::String(ParameterIds[parameter]), *component));
    component->addMouseListener(this, false);
    componentToParam[component] = parameter;
}

void CloudSeedXTAudioProcessorEditor::mouseEnter(const juce::MouseEvent& ev)
{
    if (componentToParam.count(ev.eventComponent))
    {
        auto param = componentToParam[ev.eventComponent];
        currentParameter = param;
    }
}

void CloudSeedXTAudioProcessorEditor::mouseExit(const juce::MouseEvent& ev)
{
    if (componentToParam.count(ev.eventComponent))
    {
        auto param = componentToParam[ev.eventComponent];
        if (currentParameter == param)
        {
            currentParameter = Parameter::COUNT;
        }
    }
}

CloudSeedXTAudioProcessorEditor::~CloudSeedXTAudioProcessorEditor()
{
    stopTimer();

    {
        const juce::ScopedLock myScopedLock(objectLock);
        // blocks until timer callback is done
    }
}

//==============================================================================
void CloudSeedXTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(252, 252, 252));

    juce::ColourGradient grad(juce::Colour::fromRGB(9, 44, 93), 0, 100, juce::Colour::fromRGB(22, 48, 89), 0, 600, false);
    g.setGradientFill(grad);
    g.fillRect(0, 100, 1765, 500);

    juce::ColourGradient gradTop(juce::Colour::fromFloatRGBA(0,0,0,0.3), 0, 100, juce::Colour::fromFloatRGBA(0,0,0,0), 0, 205, false);
    g.setGradientFill(gradTop);
    g.fillRect(0, 100, 1765, 105);

    g.setColour(juce::Colour::fromFloatRGBA(0.4,0.55,0.85,0.15));
    g.fillRoundedRectangle(Panel0x, 217, Panel0Width, 334, 3);
    g.fillRoundedRectangle(Panel1x, 217, Panel1Width, 334, 3);
    g.fillRoundedRectangle(Panel2x, 217, Panel2Width, 334, 3);
    g.fillRoundedRectangle(Panel3x, 217, Panel3Width, 334, 3);
    g.fillRoundedRectangle(Panel4x, 217, Panel4Width, 334, 3);

    auto img = juce::Drawable::createFromImageData(BinaryData::logo_svg, BinaryData::logo_svgSize);
    img->drawWithin(g, juce::Rectangle<float>(20, 14, 350, 66), juce::RectanglePlacement::xLeft | juce::RectanglePlacement::yTop, 1.0);

    g.setColour(ColourLightBlue);
    g.setFont(getFontLight(50));
    g.drawText("Main", Panel0x, 196, 100, 20, juce::Justification::bottomLeft, false);
    g.drawText("Taps", Panel1x, 196, 100, 20, juce::Justification::bottomLeft, false);
    g.drawText("Reflections", Panel2x, 196, 200, 20, juce::Justification::bottomLeft, false);
    g.drawText("Late Reverberation", Panel3x, 196, 450, 20, juce::Justification::bottomLeft, false);
    g.drawText("Equalization", Panel4x, 196, 350, 20, juce::Justification::bottomLeft, false);

    juce::Path p;
    p.startNewSubPath(getWidth() - ResizeTabSize, getHeight());
    p.lineTo(getWidth(), getHeight() - ResizeTabSize);
    p.lineTo(getWidth(), getHeight());
    p.closeSubPath();
    g.setColour(juce::Colour::fromHSL(0, 0, 0, 0.15));
    g.fillPath(p);
}

void CloudSeedXTAudioProcessorEditor::resized()
{
    overlay.setBounds(0, 0, getWidth(), getHeight());
}

void CloudSeedXTAudioProcessorEditor::timerCallback()
{
    if (currentParameter == Parameter::COUNT)
    {
        parameterReadout.setText("", juce::NotificationType::dontSendNotification);
        return;
    }

    auto value = audioProcessor.getParamByIdx((int)currentParameter);
    juce::String text = ParameterLabel[(int)currentParameter] + juce::String(": ") + FormatParameter(value, 32, (int)currentParameter);
    parameterReadout.setText(text, juce::NotificationType::dontSendNotification);

    //const juce::ScopedLock myScopedLock(objectLock);

    //for (int idx = 0; idx < PARAM_COUNT; idx++)
    //{
    //    if (!audioProcessor.paramDirty[idx])
    //        continue;

    //    float value = audioProcessor.getParamByIdx(idx);
    //    float scaledValue = audioProcessor.getScaledParamByIdx(idx);
    //    labelsRead[idx].setText(FormatParameter(value, 12, idx) + juce::String(" ") + juce::String(ParameterSuffix[idx]), juce::NotificationType::sendNotificationAsync);

    //    if (idx == (int)Parameters::BandUpper)
    //    {
    //        curve.upperThres = scaledValue;
    //        curve.lowerThres = audioProcessor.getScaledParamByIdx(0) - audioProcessor.getScaledParamByIdx(1);
    //    }
    //    else if (idx == (int)Parameters::BandGap) // bandgap -> lower thres
    //        curve.lowerThres = audioProcessor.getScaledParamByIdx(0) - audioProcessor.getScaledParamByIdx(1);
    //    else if (idx == (int)Parameters::Expansion)
    //        curve.expansion = scaledValue;
    //    else if (idx == (int)Parameters::Hysteresis)
    //        curve.hysteresis = scaledValue;

    //    audioProcessor.paramDirty[idx] = false;
    //}


    //auto signalInput = (120 + Clip<double>(audioProcessor.expanderL.GainSensorRO, -120.0, 0.0)) / 120.0;
    //auto expansion = -fabsf(audioProcessor.expanderL.ExpansionRO / 50.0);

    //InputMeter.setValue(signalInput);
    //ReduceMeter.setValue(expansion);
    //
    //curve.inputLevel = audioProcessor.expanderL.GainSensorRO;
    //curve.expansionLevel = audioProcessor.expanderL.ExpansionRO;
    //curve.kneeRatio = audioProcessor.expanderL.Knee;
    //curve.repaint();
}