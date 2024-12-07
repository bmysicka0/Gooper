
#pragma once

#include <juce_dsp/juce_dsp.h>
#include "HilbertProcessor.h"
#include "hilbert.h"


class FreqShifter
{
public:
    FreqShifter();

    void prepare(float sampleRate, int numChannels) noexcept;
    void setFrequency(float);
    void process(juce::AudioBuffer<float>& buffer) noexcept;

    void reset() noexcept;

private:
    using HilbertIIR = signalsmith::hilbert::HilbertIIR<float>;
    HilbertProcessor hilbertProcessor, antialiasingProcessor;

private:
    float radiansCoefficient = 0.f;
    float phase = 0.f;
    float frequencyParameter;

};