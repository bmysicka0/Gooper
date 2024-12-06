#pragma once

class StateManager;
class Gain;

#include <juce_audio_basics/juce_audio_basics.h>
#include "../audio/Flanger.h"

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

    // One or more flangers in series
    Flanger flanger1;
    Flanger flanger2;
    Flanger flanger3;
    Flanger flanger4;
    Flanger flanger5;
    Flanger flanger6;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};