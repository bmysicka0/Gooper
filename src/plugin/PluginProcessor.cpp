// Nathan Blair June 2023

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../parameters/StateManager.h"
#include "../audio/Gain.h"

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
    // Called after the constructor, but before playback starts
    // Use this to allocate up any resources you need, and to reset any
    // variables that depend on sample rate or block size

    gain = std::make_unique<Gain>(float(sampleRate), samplesPerBlock, getTotalNumOutputChannels(), PARAMETER_DEFAULTS[PARAM::GAIN] / 100.0f);

    // Store sample rate
    currentSampleRate = sampleRate;

    // Prepare the delay buffer for the flanger
    delayBufferSize = (int)(maxDelayTime * currentSampleRate) + 1;
    delayBuffer.setSize(getTotalNumOutputChannels(), delayBufferSize);
    delayBuffer.clear();
    writePosition = 0;

    // Set initial flanger parameters
    flangerRate = 0.2f;     // base rate
    flangerDepth = 0.5f;
    flangerFeedback = 0.3f;
    flangerMix = 0.5f;

    // Set up slightly different rates for L/R channels
    flangerRateLeft = flangerRate;       // Left channel LFO rate
    flangerRateRight = flangerRate * 1.02f; // Right channel slightly faster LFO

    // Reset LFO phases
    lfoPhaseLeft = 0.0f;
    lfoPhaseRight = 0.0f;

}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    //--------------------------------------------------------------------------------
    // read in some parameter values here, if you want
    // in this case, gain goes from 0 to 100 (see: ../parameters/parameters.csv)
    // so we normalize it to 0 to 1
    //--------------------------------------------------------------------------------
    auto requested_gain = state->param_value(PARAM::GAIN) / 100.0f;

    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();

    // Clear any unused output channels
    for (int ch = totalNumInputChannels; ch < totalNumOutputChannels; ++ch)
        buffer.clear(ch, 0, numSamples);

    float requestedGain = state->param_value(PARAM::GAIN) / 100.0f;
    gain->setGain(requestedGain);

    // Calculate LFO increments for left and right channels
    float lfoIncrementLeft  = (float)(2.0 * juce::MathConstants<double>::pi * flangerRateLeft / currentSampleRate);
    float lfoIncrementRight = (float)(2.0 * juce::MathConstants<double>::pi * flangerRateRight / currentSampleRate);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Advance and wrap LFO phases for left and right
        float lfoValueLeft = 0.5f * (1.0f + std::sin(lfoPhaseLeft));
        lfoPhaseLeft += lfoIncrementLeft;
        if (lfoPhaseLeft >= juce::MathConstants<float>::twoPi)
            lfoPhaseLeft -= juce::MathConstants<float>::twoPi;

        float lfoValueRight = 0.5f * (1.0f + std::sin(lfoPhaseRight));
        lfoPhaseRight += lfoIncrementRight;
        if (lfoPhaseRight >= juce::MathConstants<float>::twoPi)
            lfoPhaseRight -= juce::MathConstants<float>::twoPi;

        // Current delay times for left and right
        float currentDelayTimeLeft = maxDelayTime * flangerDepth * lfoValueLeft;
        float currentDelayTimeRight = maxDelayTime * flangerDepth * lfoValueRight;

        int delaySamplesLeft = (int)(currentDelayTimeLeft * currentSampleRate);
        int delaySamplesRight = (int)(currentDelayTimeRight * currentSampleRate);

        // Process left channel if present
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

            float wetSampleLeft = (inputSampleLeft * (1.0f - flangerMix)) + (delayedSampleLeft * flangerMix);
            channelData[sample] = wetSampleLeft;
        }

        // Process right channel if present
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

            float wetSampleRight = (inputSampleRight * (1.0f - flangerMix)) + (delayedSampleRight * flangerMix);
            channelData[sample] = wetSampleRight;
        }

        // Increment write position and wrap around
        writePosition = (writePosition + 1) % delayBufferSize;

    }

    //--------------------------------------------------------------------------------
    // process samples below. use the buffer argument that is passed in.
    // for an audio effect, buffer is filled with input samples, and you should fill it with output samples
    // for a synth, buffer is filled with zeros, and you should fill it with output samples
    // see: https://docs.juce.com/master/classAudioBuffer.html
    //--------------------------------------------------------------------------------
    
    gain->setGain(requested_gain);
    gain->process(buffer);
    //--------------------------------------------------------------------------------
    // you can use midiMessages to read midi if you need. 
    // since we are not using midi yet, we clear the buffer.
    //--------------------------------------------------------------------------------
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