/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BitCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BitCrusherAudioProcessorEditor (BitCrusherAudioProcessor&);
    ~BitCrusherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider pSlider;
    juce::Slider densitySlider;
    juce::Slider windowSlider;
    juce::Slider wetSlider;

    juce::Label pLabel;
    juce::Label dLabel;
    juce::Label winLabel;
    juce::Label wetLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> winAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetAttachment;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;



private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BitCrusherAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherAudioProcessorEditor)
};
