#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

class Flanger
{
public:
    Flanger();
    ~Flanger();

    void setParameters(float newRate, float newFeedback, bool stereoMode = true, float minFrequency = 20.0f, float maxFrequency = 2000.0f);
    void prepare (double sampleRate, int channels);
    void process (juce::AudioBuffer<float>& buffer);

private:
    double currentSampleRate;
    int totalNumChannels = 2;

    float flangerRate;
    float flangerFeedback;
    float maxDelayTime;

    bool stereoMode;

    float minFreq;    // lower bound for frequency sweep
    float maxFreq;  // upper bound for frequency sweep

    float lfoPhaseLeft;
    float lfoPhaseRight;
    float flangerRateLeft;
    float flangerRateRight;

    int delayBufferSize;
    int writePosition;

    juce::AudioBuffer<float> delayBuffer;
};
