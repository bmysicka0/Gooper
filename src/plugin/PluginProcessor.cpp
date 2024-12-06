#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../parameters/StateManager.h"
#include "../audio/Gain.h"
#include "../audio/Flanger.h"

//==============================================================================
PluginProcessor::PluginProcessor()
{
    state = std::make_unique<StateManager>(this);
}

PluginProcessor::~PluginProcessor()
{
    // stop any threads, delete any raw pointers, remove any listeners, etc
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // gain = std::make_unique<Gain>(static_cast<float>(sampleRate), samplesPerBlock, getTotalNumOutputChannels(), PARAMETER_DEFAULTS[PARAM::GAIN] / 100.0f);

    flanger1.setParameters(PARAMETER_DEFAULTS[PARAM::RATE], PARAMETER_DEFAULTS[PARAM::FEED], static_cast<bool>(PARAMETER_DEFAULTS[PARAM::STEREO]), 20.0f, 20000.0f);
    flanger1.prepare(sampleRate, getTotalNumOutputChannels());

    flanger2.setParameters(PARAMETER_DEFAULTS[PARAM::RATE] + PARAMETER_DEFAULTS[PARAM::SPREAD], 0.0f, false, 20.0f, 20000.0f);
    flanger2.prepare(sampleRate, getTotalNumOutputChannels());

    flanger3.setParameters(PARAMETER_DEFAULTS[PARAM::RATE] + PARAMETER_DEFAULTS[PARAM::SPREAD] * 2, 0.0f, false, 20.0f, 20000.0f);
    flanger3.prepare(sampleRate, getTotalNumOutputChannels());

    flanger4.setParameters(PARAMETER_DEFAULTS[PARAM::RATE] + PARAMETER_DEFAULTS[PARAM::SPREAD] * 3, PARAMETER_DEFAULTS[PARAM::FEED], static_cast<bool>(PARAMETER_DEFAULTS[PARAM::STEREO]), 20.0f, 20000.0f);
    flanger4.prepare(sampleRate, getTotalNumOutputChannels());

    flanger5.setParameters(PARAMETER_DEFAULTS[PARAM::RATE] + PARAMETER_DEFAULTS[PARAM::SPREAD] * 4, 0.0f, false, 20.0f, 20000.0f);
    flanger5.prepare(sampleRate, getTotalNumOutputChannels());

    flanger6.setParameters(PARAMETER_DEFAULTS[PARAM::RATE] + PARAMETER_DEFAULTS[PARAM::SPREAD] * 4, 0.0f, false, 20.0f, 20000.0f);
    flanger6.prepare(sampleRate, getTotalNumOutputChannels());

}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();

    // Clear extra channels if any
    for (int ch = totalNumInputChannels; ch < totalNumOutputChannels; ++ch)
        buffer.clear(ch, 0, numSamples);

    // Retrieve requested gain
    // float requestedGain = state->param_value(GAIN) / 100.0f;
    // gain->setGain(requestedGain);

    // Retrieve parameter values from state
    float rate = state->param_value(RATE);
    float feed = state->param_value(FEED);
    bool stereo = state->param_value(STEREO);
    float spread = state->param_value(SPREAD);
    float goops = state->param_value(GOOPS);

    DBG("Rate: " << rate << ", Spread: " << spread << ", Feedback: " << feed << ", Stereo: " << int(stereo));

    flanger1.setParameters(rate, feed, stereo, 20.0f, 20000.0f);
    flanger2.setParameters(rate + spread, 0.0f, false, 20.0f, 20000.0f);
    flanger3.setParameters(rate + 2.0f * spread, 0.0f, false, 20.0f, 20000.0f);
    flanger4.setParameters(rate + 3.0f * spread, feed, stereo, 20.0f, 20000.0f);
    flanger5.setParameters(rate + 4.0f * spread, 0.0f, false, 20.0f, 20000.0f);
    flanger6.setParameters(rate + 5.0f * spread, 0.0f, false, 20.0f, 20000.0f);

    // Now process through flangers with updated parameters
    if (goops >= 1) flanger1.process(buffer);
    if (goops >= 2) flanger2.process(buffer);
    if (goops >= 3) flanger3.process(buffer);
    if (goops >= 4) flanger4.process(buffer);
    if (goops >= 5) flanger5.process(buffer);

    // Now apply gain after all flanging
    // gain->process(buffer);

    // Clear MIDI if not used
    midiMessages.clear();
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // We will just store our parameter state, for now
    auto plugin_state = state->get_state();
    std::unique_ptr<juce::XmlElement> xml (plugin_state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // Restore our parameters from file
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    state->load_from(xmlState.get());
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}