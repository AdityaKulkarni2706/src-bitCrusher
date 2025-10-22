/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <random>

juce::AudioProcessorValueTreeState::ParameterLayout BitCrusherAudioProcessor::createParameterLayout() {

    juce::AudioProcessorValueTreeState::ParameterLayout layout{};

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Probability", 1 },
            "Probability",
            juce::NormalisableRange<float>(0.0f, 0.75f, 0.01f),
            0.3
        )
    );

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Density", 1 },
            "Density",
            juce::NormalisableRange<float>(0.0f, 0.75f, 0.01f),
            0.3
        )
    );

    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Window", 1 },
            "Window",
            juce::NormalisableRange<float>(0.0f, 64.0f, 1.0f),
            16.0f
        )
    );
    layout.add(
        std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{ "Wet", 1 },
            "Wet",
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
            0.5
        )
    );

    return layout; 
}

void BitCrusherAudioProcessor::applyRandomDrop(
    float* inputSamples, 
    float p,
    int numSamples
)
    
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i{ 0 }; i < numSamples; ++i) {
        if (dist(rng) < p) {

            inputSamples[i] = 0.0f;

        }
    
    }

}

void BitCrusherAudioProcessor::applyRepDrop(
    float* inputSamples,
    float p,
    int numSamples
)

{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i{ 1 }; i < numSamples; ++i) {
        if (dist(rng) < p) {

            inputSamples[i] = inputSamples[i-1];

        }

    }

}
void BitCrusherAudioProcessor::applyCDrop(
    float* inputSamples,
    float p,
    int numSamples
)

{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i{ 0 }; i < numSamples; ++i) {
        if (dist(rng) < p) {

            inputSamples[i] = 0.25;

        }

    }

}

void BitCrusherAudioProcessor::applySmoothDrop(
    float* inputSamples,
    float p,
    int numSamples,
    int window
)
    
{

    if (numSamples < 32) return;
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i{ window }; i < numSamples-window; ++i) {
        if (dist(rng) < p) {

            float avg_sample_value = 0.0f;

            for (int j{ 1 }; j < (window + 1); ++j) {
                avg_sample_value += inputSamples[i - j];
            }
            for (int j{ 1 }; j < (window + 1); ++j) {
                avg_sample_value += inputSamples[i + j];
            }


            inputSamples[i] = avg_sample_value / (2*window);

        }

    }

}


void BitCrusherAudioProcessor::applyBufferDrop(
    float* inputSamples,
    float p,
    int numSamples
)

{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    

    if (dist(rng) < p) {
        for (size_t i{ 0 }; i < numSamples; ++i) {
            inputSamples[i] = 0.0f;

        }
    }

}


//==============================================================================
BitCrusherAudioProcessor::BitCrusherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),

       apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

BitCrusherAudioProcessor::~BitCrusherAudioProcessor()
{
}

//==============================================================================
const juce::String BitCrusherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BitCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BitCrusherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BitCrusherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BitCrusherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BitCrusherAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BitCrusherAudioProcessor::getProgramName (int index)
{
    return {};
}

void BitCrusherAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BitCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BitCrusherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BitCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BitCrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float pValue = *apvts.getRawParameterValue("Probability");
    float dValue = *apvts.getRawParameterValue("Density");
    float wetValue = *apvts.getRawParameterValue("Wet");
    float windowValue = *apvts.getRawParameterValue("Window");


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto numSamples = buffer.getNumSamples();

        applySmoothDrop(channelData, pValue, numSamples, windowValue);


        // ..do something to the data...

        
        
    }
}

//==============================================================================
bool BitCrusherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BitCrusherAudioProcessor::createEditor()
{
    return new BitCrusherAudioProcessorEditor (*this);
}

//==============================================================================
void BitCrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BitCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitCrusherAudioProcessor();
}
