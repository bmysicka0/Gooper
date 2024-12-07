#pragma once

#include "hilbert.h"
#include <juce_dsp/juce_dsp.h>

// Reimplementation of Signalsmith's HilbertIIR class.
class HilbertProcessor
{
public:
    using Complex = std::complex<float>;
    using HilbertIIRCoeffs = signalsmith::hilbert::HilbertIIRCoeffs<float>;
    static constexpr int order = HilbertIIRCoeffs::order;

    HilbertProcessor() = default;

    void prepare(float sampleRate, int numChannels, float passbandGain = 2.f) noexcept;
    void reset() noexcept;

    Complex processSample(float sample, int channel) noexcept;
    Complex processSample(Complex sample, int channel) noexcept;

protected:
    using Array = std::array<float, order>;
    struct State 
    {
        Array real, imag;
    };

    Array coeffsReal, coeffsImag, polesReal, polesImag;
    std::vector<State> states;
    float direct;

};
