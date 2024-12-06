#include "Flanger.h"
#include <cmath>

Flanger::Flanger() {}
Flanger::~Flanger() {}

void Flanger::setParameters(float newRate, float newFeedback, bool stereo, float minFrequency, float maxFrequency)
{
    flangerRate = newRate;
    flangerFeedback = newFeedback;
    stereoMode = stereo;
    minFreq = minFrequency;
    maxFreq = maxFrequency;
    maxDelayTime = 1.0f / minFreq;

    flangerRateLeft = flangerRate;

    if (stereoMode)
        flangerRateRight = flangerRate * 1.02f;
    else
        flangerRateRight = flangerRate;
}

void Flanger::prepare(double sampleRate, int channels)
{
    currentSampleRate = sampleRate;
    totalNumChannels = channels;

    delayBufferSize = (int)(maxDelayTime * currentSampleRate) + 1;
    delayBuffer.setSize(totalNumChannels, delayBufferSize);
    delayBuffer.clear();
    writePosition = 0;

    lfoPhaseLeft = 0.0f;
    lfoPhaseRight = 0.0f;
}

void Flanger::process(juce::AudioBuffer<float>& buffer)
{
    int numSamples = buffer.getNumSamples();
    int totalNumOutputChannels = totalNumChannels;

    DBG("flangerRateRight: " << flangerRateRight << ", flangerRateLeft: " << flangerRateLeft );

    float lfoIncrementLeft  = (float)(2.0 * juce::MathConstants<double>::pi * flangerRateLeft / currentSampleRate);
    float lfoIncrementRight = (float)(2.0 * juce::MathConstants<double>::pi * flangerRateRight / currentSampleRate);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Compute LFO values (0.0 to 1.0)
        float lfoValueLeft = 0.5f * (1.0f + std::sin(lfoPhaseLeft));
        lfoPhaseLeft += lfoIncrementLeft;
        if (lfoPhaseLeft >= juce::MathConstants<float>::twoPi)
            lfoPhaseLeft -= juce::MathConstants<float>::twoPi;

        float lfoValueRight = 0.5f * (1.0f + std::sin(lfoPhaseRight));
        lfoPhaseRight += lfoIncrementRight;
        if (lfoPhaseRight >= juce::MathConstants<float>::twoPi)
            lfoPhaseRight -= juce::MathConstants<float>::twoPi;

        // Map LFO values to a logarithmic frequency range
        float freqLeft = minFreq * std::pow(maxFreq / minFreq, lfoValueLeft);
        float freqRight = minFreq * std::pow(maxFreq / minFreq, lfoValueRight);

        // Convert frequency to delay time
        float currentDelayTimeLeft = 1.0f / freqLeft;
        float currentDelayTimeRight = 1.0f / freqRight;

        int delaySamplesLeft = (int)(currentDelayTimeLeft * currentSampleRate);
        int delaySamplesRight = (int)(currentDelayTimeRight * currentSampleRate);

        // Process left channel
        if (totalNumOutputChannels > 0)
        {
            float* channelData = buffer.getWritePointer(0);
            float* delayData = delayBuffer.getWritePointer(0);

            int readPositionLeft = writePosition - delaySamplesLeft;
            if (readPositionLeft < 0)
                readPositionLeft += delayBufferSize;

            float delayedSampleLeft = delayData[readPositionLeft];
            float inputSampleLeft = channelData[sample];
            delayData[writePosition] = inputSampleLeft + (delayedSampleLeft * flangerFeedback);

            channelData[sample] = inputSampleLeft + delayedSampleLeft;
        }

        // Process right channel
        if (totalNumOutputChannels > 1)
        {
            float* channelData = buffer.getWritePointer(1);
            float* delayData = delayBuffer.getWritePointer(1);

            int readPositionRight = writePosition - delaySamplesRight;
            if (readPositionRight < 0)
                readPositionRight += delayBufferSize;

            float delayedSampleRight = delayData[readPositionRight];
            float inputSampleRight = channelData[sample];
            delayData[writePosition] = inputSampleRight + (delayedSampleRight * flangerFeedback);

            channelData[sample] = inputSampleRight + delayedSampleRight;
        }

        // Increment write position
        writePosition = (writePosition + 1) % delayBufferSize;
    }
}