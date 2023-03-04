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
const int PluginHeight = 700;

// global variable, affects all plugin instances, is retained until process exits
bool pluginScaleLarge = false;

//==============================================================================
CloudSeedXTAudioProcessorEditor::CloudSeedXTAudioProcessorEditor (CloudSeedXTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    const juce::ScopedLock myScopedLock(objectLock);
    currentParameter = Parameter::COUNT;
    setSize(PluginWidth, PluginHeight);
    
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

    presetName.setFont(getFontLight(42));
    presetName.setText(this->audioProcessor.getPresetName(), juce::NotificationType::dontSendNotification);
    presetName.setJustificationType(juce::Justification::centred);
    presetName.setColour(juce::Label::ColourIds::textColourId, ColourTitle);
    presetName.setBounds(PluginWidth / 2 - 300, 626, 600, 50);
    presetName.addMouseListener(this, false);
    presetName.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    addAndMakeVisible(presetName);
    

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
    i = 6;  knobs[i].setBounds(x0 + 0*w, y0, w, h);     addAndMakeVisible(knobs[i]);   attachParam(&knobs[i], Parameter::TapCount);
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
    spinnersParams[0].Formatter = [](double val) { return FormatParameter(val, 32, Parameter::EarlyDiffuseCount); };

    spinnersParams[1].setBounds(Panel3x + PanelPadding + w + 10, sy, sw, sh);
    attachParam(&spinnersParams[1], Parameter::LateLineCount);
    spinnersParams[1].Formatter = [](double val) { return FormatParameter(val, 32, Parameter::LateLineCount); };

    spinnersParams[2].setBounds(Panel3x + PanelPadding + 3*w + 10, sy, sw, sh);
    attachParam(&spinnersParams[2], Parameter::LateDiffuseCount);
    spinnersParams[2].Formatter = [](double val) { return FormatParameter(val, 32, Parameter::LateDiffuseCount); };

    spinnersParams[0].setMouseCursor(juce::MouseCursor::BottomEdgeResizeCursor);
    spinnersParams[1].setMouseCursor(juce::MouseCursor::BottomEdgeResizeCursor);
    spinnersParams[2].setMouseCursor(juce::MouseCursor::BottomEdgeResizeCursor);

    addAndMakeVisible(spinnersParams[0]);
    addAndMakeVisible(spinnersParams[1]);
    addAndMakeVisible(spinnersParams[2]);
    
    spinToggleButtons[0].setBounds(Panel0x + PanelPadding + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[0], Parameter::Interpolation);
    spinToggleButtons[1].setBounds(Panel0x + PanelPadding + w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[1], Parameter::LowCutEnabled);
    spinToggleButtons[2].setBounds(Panel0x + PanelPadding + 2*w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[2], Parameter::HighCutEnabled);

    spinToggleButtons[3].setBounds(Panel1x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[3], Parameter::TapEnabled);

    spinToggleButtons[4].setBounds(Panel2x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[4], Parameter::EarlyDiffuseEnabled);

    spinToggleButtons[5].setBounds(Panel3x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[5], Parameter::LateMode);
    spinToggleButtons[5].OffValue = "Pre";
    spinToggleButtons[5].OnValue = "Post";
    spinToggleButtons[6].setBounds(Panel3x + PanelPadding + 2 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[6], Parameter::LateDiffuseEnabled);

    spinToggleButtons[7].setBounds(Panel4x + PanelPadding + 0 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[7], Parameter::EqLowShelfEnabled);
    spinToggleButtons[8].setBounds(Panel4x + PanelPadding + 1 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[8], Parameter::EqHighShelfEnabled);
    spinToggleButtons[9].setBounds(Panel4x + PanelPadding + 2 * w + 10, sy, sw, sh);
    attachParam(&spinToggleButtons[9], Parameter::EqLowpassEnabled);

    for (int i = 0; i < 10; i++)
    {
        addAndMakeVisible(spinToggleButtons[i]);
        spinToggleButtons[i].setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    pluginName.setText("Cloud Seed XT", juce::NotificationType::dontSendNotification);
    pluginName.setFont(getFontLight(70));
    pluginName.setJustificationType(juce::Justification::centredTop);
    pluginName.setColour(juce::Label::ColourIds::textColourId, ColourTitle);
    pluginName.setBounds(PluginWidth/2 - 300, 10, 600, 70);
    addAndMakeVisible(pluginName);

    aboutDialog.setBounds(0, 0, 550, 180);
    aboutDialog.SetOverlay(&overlay);
    overlay.setComponent(&aboutDialog);
    overlay.setBounds(0, 0, PluginWidth, PluginHeight);
    addAndMakeVisible(overlay);
    overlay.setVisible(false);
    
    startTimerHz(30);

    // Adjust the About dialog to compensate for small scaling factor
    overlay.setTransform(juce::AffineTransform::scale(1.5, 1.5, PluginWidth / 2, PluginHeight / 2));
}

CloudSeedXTAudioProcessorEditor::~CloudSeedXTAudioProcessorEditor()
{
    stopTimer();
    {
        const juce::ScopedLock myScopedLock(objectLock);
        // blocks until timer callback is done
    }
}

void CloudSeedXTAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(ColourAlmostWhite);

    juce::ColourGradient grad(juce::Colour::fromRGB(9, 44, 93), 0, 100, juce::Colour::fromRGB(22, 48, 89), 0, 600, false);
    g.setGradientFill(grad);
    g.fillRect(0, 100, PluginWidth, 500);
    drawNoise(g, 0, 100, PluginWidth, 500);

    juce::ColourGradient gradTop(juce::Colour::fromFloatRGBA(0, 0, 0, 0.3), 0, 100, juce::Colour::fromFloatRGBA(0, 0, 0, 0), 0, 205, false);
    g.setGradientFill(gradTop);
    g.fillRect(0, 100, PluginWidth, 105);

    g.setColour(juce::Colour::fromFloatRGBA(0.4, 0.55, 0.85, 0.15));
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
    p.startNewSubPath(PluginWidth - ResizeTabSize, PluginHeight);
    p.lineTo(PluginWidth, PluginHeight - ResizeTabSize);
    p.lineTo(PluginWidth, PluginHeight);
    p.closeSubPath();
    g.setColour(juce::Colour::fromHSL(0, 0, 0, 0.15));
    g.fillPath(p);
}

void CloudSeedXTAudioProcessorEditor::resized()
{
    overlay.setBounds(0, 0, getWidth(), getHeight());
}

void CloudSeedXTAudioProcessorEditor::mouseMove(const juce::MouseEvent& event)
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

void CloudSeedXTAudioProcessorEditor::mouseDown(const juce::MouseEvent& event)
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
    {
        pluginScaleLarge = !pluginScaleLarge;
        applyEditorScaling();
    }
}

void CloudSeedXTAudioProcessorEditor::mouseEnter(const juce::MouseEvent& ev)
{
    if (ev.eventComponent == &presetName)
        presetName.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour::fromRGB(240, 240, 245));
    

    if (componentToParam.count(ev.eventComponent))
    {
        auto param = componentToParam[ev.eventComponent];
        currentParameter = param;
    }
}

void CloudSeedXTAudioProcessorEditor::mouseExit(const juce::MouseEvent& ev)
{
    if (ev.eventComponent == &presetName)
        presetName.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::transparentWhite);

    if (componentToParam.count(ev.eventComponent))
    {
        auto param = componentToParam[ev.eventComponent];
        if (currentParameter == param)
        {
            currentParameter = Parameter::COUNT;
        }
    }
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

void CloudSeedXTAudioProcessorEditor::showAboutDialog()
{
    overlay.setComponent(&aboutDialog);
    overlay.setVisible(true);
}

void CloudSeedXTAudioProcessorEditor::reloadPresetName()
{
    presetName.setText(this->audioProcessor.getPresetName(), juce::NotificationType::dontSendNotification);
}

void CloudSeedXTAudioProcessorEditor::savePreset()
{
    auto cbInner = [this](std::string value, int btnIndex)
    {
        if (btnIndex == 0)
        {
            Presets::savePreset(&this->audioProcessor, value);
            reloadPresetName();
        }

        overlay.setVisible(false);
    };

    std::vector<std::string> buttons;
    buttons.push_back("Save");
    buttons.push_back("Cancel");
    dialog.setContent("Save Preset", buttons, cbInner);
    overlay.setComponent(&dialog);
    overlay.setVisible(true);
}

void CloudSeedXTAudioProcessorEditor::applyEditorScaling()
{
    if (pluginScaleLarge)
        setScaleFactor(1.0);
    else
        setScaleFactor(0.75);

    repaint();
}