/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditorGUI.h"

//==============================================================================
XDelayAudioProcessor::XDelayAudioProcessor() :
    ppvts(*this, nullptr, Identifier("XDelayParams"), {
            std::make_unique<AudioParameterFloat>(NAME_PAN, "Pan", -1.0f, 1.0f, DEFAULT_PAN),
            std::make_unique<AudioParameterFloat>(NAME_DELAY, "Delay", 0.0f, MAX_DELAY_TIME, DEFAULT_DELAY),
            std::make_unique<AudioParameterFloat>(NAME_FEEDBACK, "Feedback", 0.0f, 1.0f, DEFAULT_FEEDBACK),
            std::make_unique<AudioParameterFloat>(NAME_TONE, "Tone", -1.0f, 1.0f, DEFAULT_TONE),
            std::make_unique<AudioParameterFloat>(NAME_MIX, "Mix", 0.0f, 1.0f, DEFAULT_MIX),
            std::make_unique<AudioParameterBool>(NAME_BYPASS, "Bypass", DEFAULT_BYPASS)
    })
{
    ppvts.addParameterListener(NAME_PAN, &panner);
    ppvts.addParameterListener(NAME_FEEDBACK, &delay);
    ppvts.addParameterListener(NAME_DELAY, &delay);
    ppvts.addParameterListener(NAME_TONE, &filter);
    ppvts.addParameterListener(NAME_MIX, &mix);
}

XDelayAudioProcessor::~XDelayAudioProcessor()
{
}

//==============================================================================
void XDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mix.prepareToPlay(samplesPerBlock);
    delay.prepareToPlay(samplesPerBlock, sampleRate);
    panner.prepareToPlay(sampleRate);
    filter.prepareToPlay(sampleRate);

}

void XDelayAudioProcessor::releaseResources()
{
    delay.releaseResources();
    mix.releaseResources();
    filter.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool XDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void XDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mix.storeFrom(buffer);
    delay.processBlock(buffer);
    filter.processBlock(buffer, buffer.getNumSamples());
    mix.mergeTo(buffer);
    panner.processBlock(buffer, getTotalNumInputChannels(), getTotalNumOutputChannels());

}

// GRAPHIC EDITOR
//==============================================================================
bool XDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}
juce::AudioProcessorEditor* XDelayAudioProcessor::createEditor()
{
    return new PluginEditorGUI(*this, ppvts);
}

// STATE INFORMATION
//==============================================================================
void XDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = ppvts.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}
void XDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(ppvts.state.getType()))
            ppvts.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new XDelayAudioProcessor();
}
