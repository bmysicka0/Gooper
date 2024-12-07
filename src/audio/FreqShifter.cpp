#include "FreqShifter.h"

FreqShifter::FreqShifter() : phase(0.0f), frequencyParameter(0.0f), radiansCoefficient(0.0f) {}

void FreqShifter::prepare(float sampleRate, int numChannels) noexcept
{
    hilbertProcessor.prepare(sampleRate, numChannels);
    antialiasingProcessor.prepare(sampleRate, numChannels, 1.0f); // Assuming passbandGain is 1.0f
    radiansCoefficient = 2.0f * static_cast<float>(M_PI) / sampleRate;
}

void FreqShifter::setFrequency(float freq)
{
    frequencyParameter = freq;
}

void FreqShifter::process(juce::AudioBuffer<float>& buffer) noexcept
{
    const float frequency = frequencyParameter;
    const float phaseDelta = frequency * radiansCoefficient;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        float currentPhase = phase;

        for (int i = 0; i < numSamples; ++i)
        {
            float inputSample = channelData[i];

            // Hilbert Transform
            auto filteredSample = hilbertProcessor.processSample(inputSample, channel);

            // Heterodyne/ringmod
            std::complex<float> phaser = std::polar(1.0f, currentPhase);
            auto shiftedSample = filteredSample * phaser;

            // Anti-aliasing
            auto outputSample = antialiasingProcessor.processSample(shiftedSample, channel);

            // Add the wet (processed) signal to the input signal
            channelData[i] = inputSample + outputSample.real();

            // Update phase
            currentPhase += phaseDelta;
        }
    }

    // Wrap the phase to prevent overflow
    phase = std::fmod(phase + numSamples * phaseDelta, 2.0f * static_cast<float>(M_PI));
}


void FreqShifter::reset() noexcept
{
    hilbertProcessor.reset();
    antialiasingProcessor.reset();
    phase = 0.0f;
}
