#ifndef DRY_WET_PROCESSOR_H
#define DRY_WET_PROCESSOR_H

#include <juce_audio_basics/juce_audio_basics.h>

class DryWet
{
public:
    DryWet() = default;

    // Set the dry/wet mix parameter (0.0 = fully dry, 1.0 = fully wet)
    void setMix(float newMix) noexcept;

    // Apply the dry/wet mix to the audio buffer
    void apply(juce::AudioBuffer<float>& dryBuffer, juce::AudioBuffer<float>& wetBuffer) const noexcept;

private:
    float mix = 0.5f; // Default to an equal mix
};

#endif // DRY_WET_PROCESSOR_H
