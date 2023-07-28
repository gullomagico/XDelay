/*
  ==============================================================================

    Delay.h
    Created: 24 Jan 2022 4:52:57pm
    Author:  GulloMagico

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

#define FB_SMOOTHING_TIME   0.01f
#define TIME_SMOOTHING_TIME 0.4f

using namespace juce;

class DigitalDelayLine : public AudioProcessorValueTreeState::Listener
{
public:
	DigitalDelayLine() {};
	~DigitalDelayLine() {};

	void prepareToPlay(const int maxSamplesPerBlock, const double sampleRate)
	{
		sr = sampleRate;
		const auto delayBufferSize = MAX_DELAY_TIME * sampleRate + maxSamplesPerBlock;
		delayBuffer.setSize(2, delayBufferSize);
	}

	void releaseResources()
	{
		delayBuffer.setSize(0, 0);
	}

	void processBlock(AudioBuffer<float>& buffer)
	{
		pullFrom(buffer);
		pushTo(buffer);
		updatePosition(buffer.getNumSamples());
	}

protected:
	void pullFrom(const AudioBuffer<float>& inputSignal)
	{
		const auto numCh = inputSignal.getNumChannels();
		const auto numInputSamples = inputSignal.getNumSamples();
		const auto delayBufferSize = delayBuffer.getNumSamples();

		for (int ch = 0; ch < numCh; ++ch)
			if (writePosition + numInputSamples <= delayBufferSize)
				delayBuffer.copyFrom(ch, writePosition, inputSignal, ch, 0, numInputSamples);
			else
			{
				const auto fittingSamples = delayBufferSize - writePosition;
				const auto remainingSamples = numInputSamples - fittingSamples;
				delayBuffer.copyFrom(ch, writePosition, inputSignal, ch, 0, fittingSamples);
				delayBuffer.copyFrom(ch, 0, inputSignal, ch, fittingSamples, remainingSamples);
			}
	}

	void pushTo(AudioBuffer<float>& outputSignal)
	{
		const auto numCh = outputSignal.getNumChannels();
		const auto numOutputSamples = outputSignal.getNumSamples();
		const auto delayBufferSize = delayBuffer.getNumSamples();

		const auto readPosition = (writePosition - roundToInt(delayTime * sr) + delayBufferSize) % delayBufferSize;

		for (int ch = 0; ch < numCh; ++ch)
		{
			// Copio il segnale ritardato sull'output
			if (readPosition + numOutputSamples <= delayBufferSize)
				outputSignal.copyFrom(ch, 0, delayBuffer, ch, readPosition, numOutputSamples);
			else
			{
				const auto fittingSamples = delayBufferSize - readPosition;
				const auto remainingSamples = numOutputSamples - fittingSamples;
				outputSignal.copyFrom(ch, 0, delayBuffer, ch, readPosition, fittingSamples);
				outputSignal.copyFrom(ch, fittingSamples, delayBuffer, ch, 0, remainingSamples);
			}

			// Aggiungo il segnale ritardato sul delayBuffer (alla posizione di scrittura)
			if (numOutputSamples < delayTime * sr)
				if (readPosition + numOutputSamples <= delayBufferSize)
					delayBuffer.addFrom(ch, writePosition, outputSignal, ch, 0, numOutputSamples, feedback);
				else
				{
					const auto fittingSamples = delayBufferSize - readPosition;
					const auto remainingSamples = numOutputSamples - fittingSamples;
					delayBuffer.addFrom(ch, writePosition, outputSignal, ch, 0, fittingSamples, feedback);
					delayBuffer.addFrom(ch, 0, outputSignal, ch, fittingSamples, remainingSamples, feedback);
				}
		}
	}

	void updatePosition(const int leap)
	{
		writePosition += leap;
		writePosition %= delayBuffer.getNumSamples();
	}

	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_DELAY)
			delayTime = newValue;
		if (paramID == NAME_FEEDBACK)
			feedback = newValue;
	}

	int writePosition = 0;
	float delayTime = DEFAULT_DELAY;
	float feedback = DEFAULT_FEEDBACK;
	double sr = 44100.0;

	AudioBuffer<float> delayBuffer;
};

class AnalogDelayLine : public AudioProcessorValueTreeState::Listener
{
public:
	AnalogDelayLine()
	{
		smoothedTime.setCurrentAndTargetValue(DEFAULT_DELAY);
		smoothedFeedback.setCurrentAndTargetValue(DEFAULT_FEEDBACK);
	}

	~AnalogDelayLine() {};

	void prepareToPlay(const int maxSamplesPerBlock, const double sampleRate)
	{
		sr = sampleRate;
		const auto delayBufferSize = MAX_DELAY_TIME * sampleRate + maxSamplesPerBlock;
		delayBuffer.setSize(2, delayBufferSize);

		smoothedFeedback.reset(sr, FB_SMOOTHING_TIME);
		smoothedTime.reset(sr, TIME_SMOOTHING_TIME);
	}

	void releaseResources()
	{
		delayBuffer.setSize(0, 0);
	}

	void processBlock(AudioBuffer<float>& buffer)
	{
		pullFrom(buffer);
		pushTo(buffer);
		updatePosition(buffer.getNumSamples());
	}

private:
	void pullFrom(const AudioBuffer<float>& inputSignal)
	{
		const auto numCh = inputSignal.getNumChannels();
		const auto numInputSamples = inputSignal.getNumSamples();
		const auto delayBufferSize = delayBuffer.getNumSamples();

		for (int ch = 0; ch < numCh; ++ch)
			if (writePosition + numInputSamples <= delayBufferSize)
				delayBuffer.copyFrom(ch, writePosition, inputSignal, ch, 0, numInputSamples);
			else
			{
				const auto fittingSamples = delayBufferSize - writePosition;
				const auto remainingSamples = numInputSamples - fittingSamples;
				delayBuffer.copyFrom(ch, writePosition, inputSignal, ch, 0, fittingSamples);
				delayBuffer.copyFrom(ch, 0, inputSignal, ch, fittingSamples, remainingSamples);
			}
	}

	void pushTo(AudioBuffer<float>& outputSignal)
	{
		const auto numCh = outputSignal.getNumChannels();
		const auto numOutputSamples = outputSignal.getNumSamples();
		const auto delayBufferSize = delayBuffer.getNumSamples();

		auto** delayData = delayBuffer.getArrayOfWritePointers();
		auto** outData = outputSignal.getArrayOfWritePointers();

		for (int smp = 0; smp < numOutputSamples; ++smp)
		{
			const auto fb = smoothedFeedback.getNextValue();
			const auto dt = smoothedTime.getNextValue();

			const auto actualWritePosition = (writePosition + smp) % delayBufferSize;

			const float delayInSamples = dt * sr;
			const float actualReadPosition = ((float)actualWritePosition - delayInSamples);

			const int integerPart = (int)floor(actualReadPosition);
			const float fractionalPart = actualReadPosition - (float)integerPart;

			const int A = (integerPart + delayBufferSize) % delayBufferSize;
			const int B = (A + 1) % delayBufferSize;

			// auto alpha = fractionalPart / (2.0f - fractionalPart); // Usato per il filtro allpass

			for (int ch = 0; ch < numCh; ++ch)
			{
				// Leggo dal buffer del delay

				//	Arrotondamento (no fractional delay)
				// const auto sampleData = delayData[ch][A]; 

				//	Interp. Lineare
				const auto sampleData = delayData[ch][A] * (1.0f - fractionalPart) + delayData[ch][B] * fractionalPart;

				//  Allpass
				// const auto sampleData = alpha * (delayData[ch][B] - oldOutput[ch]) + delayData[ch][A];
				// oldOutput[ch] = sampleData;

				// Scrivo in output
				outData[ch][smp] = sampleData;

				// Feedback
				delayData[ch][actualWritePosition] += sampleData * fb;
			}
		}


	}

	void updatePosition(const int leap)
	{
		writePosition += leap;
		writePosition %= delayBuffer.getNumSamples();
	}

	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_DELAY)
			smoothedTime.setTargetValue(newValue);
		if (paramID == NAME_FEEDBACK)
			smoothedFeedback.setTargetValue(newValue);
	}

	int writePosition = 0;
	double sr = 44100.0;

	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedFeedback;
	SmoothedValue<float, ValueSmoothingTypes::Linear> smoothedTime;

	// float oldOutput[2] = {0.0f, 0.0f}; // Array che contiene l'output precedente di entrambi i canali, usato per il filtro allpass

	AudioBuffer<float> delayBuffer;

};