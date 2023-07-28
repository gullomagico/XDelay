/*
  ==============================================================================

    Mix.h
    Created: 25 Jan 2022 1:09:12am
    Author:  GulloMagico

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

using namespace juce;

class Mix : public AudioProcessorValueTreeState::Listener
{
public:
	Mix()
	{
		updateInternalState(DEFAULT_MIX);
	}

	~Mix() {};

	void prepareToPlay(const int maxSamplesPerBlock)
	{
		drySignal.setSize(2, maxSamplesPerBlock);
	}

	void releaseResources()
	{
		drySignal.setSize(0, 0);
	}

	void storeFrom(const AudioBuffer<float>& inputSignal)
	{
		const auto numSamples = inputSignal.getNumSamples();

		for (int ch = inputSignal.getNumChannels(); --ch >= 0;)
			drySignal.copyFrom(ch, 0, inputSignal, ch, 0, numSamples);

		// se avessi mono-in stereo-out potrei tenere drySignal sempre stereo e fare
		// sourceCh = jmin(ch, inputSignal.getNumChannels()-1);
		// drySignal.copyFrom(ch, 0, inputSignal, sourceCh, 0, numSamples);
	}

	void mergeTo(AudioBuffer<float>& outputSignal)
	{
		const auto numSamples = outputSignal.getNumSamples();

		outputSignal.applyGain(wetGain);

		for (int ch = outputSignal.getNumChannels(); --ch >= 0;)
			outputSignal.addFrom(ch, 0, drySignal, ch, 0, numSamples, dryGain);
	}

private:
	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_MIX)
			updateInternalState(newValue);
	}

	void updateInternalState(float newValue)
	{
		dryGain = sqrt(1.0f - newValue);
		wetGain = sqrt(newValue);
	}

	AudioBuffer<float> drySignal;

	float dryGain;
	float wetGain;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mix)
};
