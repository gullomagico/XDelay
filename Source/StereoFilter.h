/*
  ==============================================================================

    StereoFilter.h
    Created: 25 Jan 2022 2:06:19am
    Author:  GulloMagico

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

using namespace juce;

#define DEFAULT_NUM_FILTERS 2


class StereoFilter : public AudioProcessorValueTreeState::Listener
{
public:
	StereoFilter()
	{
		for (int i = 0; i < DEFAULT_NUM_FILTERS; ++i)
			iirFilters.add(new IIRFilter());
	}

	~StereoFilter() {}

	void prepareToPlay(double sr)
	{
		sampleRate = sr;
		for (int i = iirFilters.size(); --i >= 0;)
			iirFilters.getUnchecked(i)->reset();

		updateCoefficients();
	}

	void releaseResources()
	{
		iirFilters.clear(true);
	}

	void processBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		for (int ch = buffer.getNumChannels(); --ch >= 0;)
			iirFilters.getUnchecked(ch)->processSamples(buffer.getWritePointer(ch), numSamples);
	}

	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_TONE)
			tone = newValue;

		
		updateCoefficients();
	}

private:

	void updateCoefficients()
	{
		if (tone != 0.0f)
		{ 
			IIRCoefficients coefficients;

			if (tone >= 0.0f) 
			{
				freqT = abs(tone) * CROSS_FREQ;
				coefficients = IIRCoefficients::makeHighPass(sampleRate, freqT, DEFAULT_Q);
			}
			else
			{
				freqT = (20000.0 - CROSS_FREQ) * (1.0 - abs(tone)) + CROSS_FREQ;
				coefficients = IIRCoefficients::makeLowPass(sampleRate, freqT, DEFAULT_Q);
			}

			for (int i = iirFilters.size(); --i >= 0;)
				iirFilters.getUnchecked(i)->setCoefficients(coefficients);
		}
	}

	double sampleRate = 44100.0;
	double freqT = 0.0;
	float tone = DEFAULT_TONE;

	OwnedArray<IIRFilter> iirFilters;
};