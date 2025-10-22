/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BitCrusherAudioProcessorEditor::BitCrusherAudioProcessorEditor (BitCrusherAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (450, 350);

    pSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    densitySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    windowSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    wetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);

    pSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    densitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    windowSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    pSlider.setRange(0, 0.75, 0.01);
    densitySlider.setRange(0, 0.75, 0.01);
    windowSlider.setRange(0, 64, 1);
    wetSlider.setRange(0, 1, 0.01);

    pSlider.setValue(0.3);
    densitySlider.setValue(0.3);
    windowSlider.setValue(16);
    wetSlider.setValue(0.5);

    pLabel.setText("Probability", juce::dontSendNotification);
    dLabel.setText("Density", juce::dontSendNotification);
    winLabel.setText("Window", juce::dontSendNotification);
    wetLabel.setText("Wet", juce::dontSendNotification);

    pLabel.setJustificationType(juce::Justification::centred);
    dLabel.setJustificationType(juce::Justification::centred);
    winLabel.setJustificationType(juce::Justification::centred);
    wetLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(pSlider);
    addAndMakeVisible(densitySlider);
    addAndMakeVisible(windowSlider);
    addAndMakeVisible(wetSlider);


    addAndMakeVisible(pLabel);
    addAndMakeVisible(dLabel);
    addAndMakeVisible(winLabel);
    addAndMakeVisible(wetLabel);

    pAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "Probability", pSlider
    );

    dAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "Density", densitySlider
    );

    winAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "Window", windowSlider
    );

    wetAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "Wet", wetSlider
    );









}

BitCrusherAudioProcessorEditor::~BitCrusherAudioProcessorEditor()
{
}

//==============================================================================
void BitCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void BitCrusherAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds().reduced(20);
    int sliderSize = 100;


    int topMargin = 75;
    int labelHeight = 20;
    int totalHeight = sliderSize * 2 + topMargin + 2 * labelHeight;
    int totalWidth = sliderSize * 2 + 80;

    pSlider.setBounds(
        40,
        topMargin,
        sliderSize,
        sliderSize
    );

    densitySlider.setBounds(
        getWidth() - sliderSize - 40,
        topMargin,
        sliderSize,
        sliderSize
    );

    windowSlider.setBounds(
        40,
        getHeight() - 40 - sliderSize,
        sliderSize,
        sliderSize
    );

    wetSlider.setBounds(

        getWidth() - sliderSize - 40,
        getHeight() - 40 - sliderSize,
        sliderSize,
        sliderSize

    );

    pLabel.setBounds(
        pSlider.getX(),
        pSlider.getBottom() + 5,

        sliderSize,
        labelHeight

    );

    winLabel.setBounds(
    windowSlider.getX(),
    windowSlider.getBottom() + 5,

        sliderSize,
        labelHeight

    );

    dLabel.setBounds(
        densitySlider.getX(),
        densitySlider.getBottom() + 5,

        sliderSize,
        labelHeight

    );

    wetLabel.setBounds(
        wetSlider.getX(),
        wetSlider.getBottom() + 5,

        sliderSize,
        labelHeight

    );

}
