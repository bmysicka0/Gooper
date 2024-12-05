// Nathan Blair June 2023

#pragma once

class StateManager;
class Gain;

#include <juce_audio_basics/juce_audio_basics.h>

#include "PluginProcessorBase.h"

//==============================================================================
class PluginProcessor  : public PluginProcessorBase
{
public:
    //==============================================================================
    PluginProcessor();
    ~PluginProcessor() override;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    //==============================================================================
    // state
    //==============================================================================
    std::unique_ptr<StateManager> state;

  private:

    std::unique_ptr<Gain> gain;

    // Flanger parameters
    double currentSampleRate = 0.0;
    float flangerRate = 0.2f;       // base LFO rate in Hz
    float flangerDepth = 0.5f;      // fraction of maxDelayTime used
    float flangerFeedback = 0.3f;   // feedback amount
    float flangerMix = 0.5f;        // wet/dry balance

    float lfoPhaseLeft = 0.0f;
    float lfoPhaseRight = 0.0f;

    float flangerRateLeft = 0.2f;
    float flangerRateRight = 0.204f; // slightly different rate for the right channel

    float maxDelayTime = 0.01f;     // max delay time in seconds (10ms)

    int delayBufferSize = 0;
    int writePosition = 0;
    juce::AudioBuffer<float> delayBuffer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};