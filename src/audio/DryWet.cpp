#include "DryWet.h"

void DryWet::setMix(float newMix) noexcept
{
    mix = juce::jlimit(0.0f, 1.0f, newMix); // Ensure the value is clamped between 0 and 1
}

void DryWet::apply(juce::AudioBuffer<float>& dryBuffer, juce::AudioBuffer<float>& wetBuffer) const noexcept
{
    // Ensure the buffers are compatible
    jassert(dryBuffer.getNumChannels() == wetBuffer.getNumChannels());
    jassert(dryBuffer.getNumSamples() == wetBuffer.getNumSamples());

    int numChannels = dryBuffer.getNumChannels();
    int numSamples = dryBuffer.getNumSamples();

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* wetChannel = wetBuffer.getWritePointer(channel);
        const float* dryChannel = dryBuffer.getReadPointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            // Blend dry and wet signals
            wetChannel[sample] = (1.0f - mix) * dryChannel[sample] + mix * wetChannel[sample];
        }
    }
}
