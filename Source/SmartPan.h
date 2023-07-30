/*
  ==============================================================================

    SmartPan.h
    Created: 24 Jan 2022 2:44:16am
    Author:  GulloMagico

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

using namespace juce;

#define RAMP_LEN 0.03f

class SmartPan : public AudioProcessorValueTreeState::Listener
{
public:
	SmartPan() {};
	~SmartPan() {};

	void prepareToPlay(double sampleRate)
	{
		gainLeft.reset(sampleRate, RAMP_LEN);
		gainRight.reset(sampleRate, RAMP_LEN);
		gainMono.reset(sampleRate, RAMP_LEN);

		updateInternalState();
	};

	void processBlock(AudioBuffer<float>& buffer, int numInCh, int numOutCh)
	{
		auto numSamples = buffer.getNumSamples();

		if (numInCh < numOutCh) // Mono In - Stereo Out
			buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);

		if (numInCh + numOutCh == 2) // Mono In - Mono Out
			gainMono.applyGain(buffer, numSamples);
		else // Stero In - Stereo Out || Mono In - Stereo Out || Stereo In - Mono Out
		{
			auto channelsData = buffer.getArrayOfWritePointers();
			gainLeft.applyGain(channelsData[0], numSamples);
			gainRight.applyGain(channelsData[1], numSamples);
		}

		if (numInCh > numOutCh) // Stereo In - Mono Out
			buffer.addFrom(0, 0, buffer, 1, 0, numSamples);
	};

private:
	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_PAN)
			panParam = newValue;

		updateInternalState();
	};

	void updateInternalState()
	{
		auto pan = 0.5f * (panParam + 1.0f);

		auto levelChange = (1.0f / sqrt(2.0f));

		gainMono.setTargetValue(levelChange);
		gainLeft.setTargetValue(levelChange * sqrt(1.0f - pan));
		gainRight.setTargetValue(levelChange * sqrt(pan));
	};

	float panParam = DEFAULT_PAN;

	SmoothedValue<float, ValueSmoothingTypes::Linear> gainLeft;
	SmoothedValue<float, ValueSmoothingTypes::Linear> gainRight;
	SmoothedValue<float, ValueSmoothingTypes::Linear> gainMono;
};