// Nathan Blair January 2023

#include "PluginEditor.h"
#include "../parameters/StateManager.h"
#include "../interface/ParameterSlider.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    state = processorRef.state.get();
    startTimerHz(int(TIMER_HZ));

    // INIT UNDO/REDO
    undo_manager = state->get_undo_manager();

    // add slider BEFORE setting size
    gain_slider = std::make_unique<ParameterSlider>(state, PARAM::GAIN);
    addAndMakeVisible(*gain_slider);

    goops_slider = std::make_unique<ParameterSlider>(state, PARAM::GOOPS);
    addAndMakeVisible(*goops_slider);

    feed_slider = std::make_unique<ParameterSlider>(state, PARAM::FEED);
    addAndMakeVisible(*feed_slider);

    rate_slider = std::make_unique<ParameterSlider>(state, PARAM::RATE);
    addAndMakeVisible(*rate_slider);

    spread_slider = std::make_unique<ParameterSlider>(state, PARAM::SPREAD);
    addAndMakeVisible(*spread_slider);

    stereo_slider = std::make_unique<ParameterSlider>(state, PARAM::STEREO);
    addAndMakeVisible(*stereo_slider);

    shepard_slider = std::make_unique<ParameterSlider>(state, PARAM::SHEPARD);
    addAndMakeVisible(*shepard_slider);

    goopage_slider = std::make_unique<ParameterSlider>(state, PARAM::GOOPAGE);
    addAndMakeVisible(*goopage_slider);

    drywet_slider = std::make_unique<ParameterSlider>(state, PARAM::DRYWET);
    addAndMakeVisible(*drywet_slider);

    // some settings about UI
    setOpaque (true);
    setSize(W, H);
    setColour(0, juce::Colour(0xff00ffa1)); // background color
    
    // resizable window
    setResizable(true, true);
    setResizeLimits((W * 4) / 5, (H * 4) / 5, (W * 3) / 2, (H * 3) / 2);
    getConstrainer()->setFixedAspectRatio(float(W) / float(H));
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    // remove any listeners here

    // also, if we have a lookAndFeel object we should call:
    // setLookAndFeel(nullptr); 
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Our component is opaque, so we must completely fill the background with a solid colour
    g.fillAll(findColour(0));
}

void AudioPluginAudioProcessorEditor::resized()
{
    // set the position of your components here
    int slider_size = proportionOfWidth(0.1f);
    int slider_x = proportionOfWidth(0.1f) - (slider_size / 2);
    int slider_y = proportionOfHeight(0.5f) - (slider_size / 2);
    gain_slider->setBounds(slider_x, slider_y, slider_size, slider_size);

    int goops_slider_x = slider_x + slider_size;
    goops_slider->setBounds(goops_slider_x, slider_y, slider_size, slider_size);

    int feed_slider_x = goops_slider_x + slider_size;
    feed_slider->setBounds(feed_slider_x, slider_y, slider_size, slider_size);

    int rate_slider_x = feed_slider_x + slider_size;
    rate_slider->setBounds(rate_slider_x, slider_y, slider_size, slider_size);

    int spread_slider_x = rate_slider_x + slider_size;
    spread_slider->setBounds(spread_slider_x, slider_y, slider_size, slider_size);

    int stereo_slider_x = spread_slider_x + slider_size;
    stereo_slider->setBounds(stereo_slider_x, slider_y, slider_size, slider_size);

    int shepard_slider_x = stereo_slider_x + slider_size;
    shepard_slider->setBounds(shepard_slider_x, slider_y, slider_size, slider_size);

    int goopage_slider_x = shepard_slider_x + slider_size;
    goopage_slider->setBounds(goopage_slider_x, slider_y, slider_size, slider_size);

    int drywet_slider_x = goopage_slider_x + slider_size;
    drywet_slider->setBounds(drywet_slider_x, slider_y, slider_size, slider_size);
}

void AudioPluginAudioProcessorEditor::timerCallback() {
    // repaint UI and note that we have updated ui, if parameter values have changed
    if (state->any_parameter_changed.exchange(false)) {
        if (state->get_parameter_modified(PARAM::GAIN)) {
            gain_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::GOOPS)) {
            goops_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::FEED)) {
            feed_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::RATE)) {
            rate_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::SPREAD)) {
            spread_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::STEREO)) {
            stereo_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::SHEPARD)) {
            shepard_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::GOOPAGE)) {
            goopage_slider->repaint();
        }
        if (state->get_parameter_modified(PARAM::DRYWET)) {
            drywet_slider->repaint();
        }
    }
    state->update_preset_modified();

    if (timer_counter % (TIMER_HZ / UNDO_HZ) == 0 ) {
        if (!isMouseButtonDownAnywhere()) {
            processorRef.state->get_undo_manager()->beginNewTransaction();
        }
    }

    timer_counter++;
}