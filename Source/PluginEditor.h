#pragma once

#include <juce_graphics/fonts/harfbuzz/hb-cplusplus.hh>

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ResponseCurveComponent.h"

using namespace std;



class TwoBandEQAudioProcessorEditor  : public juce::AudioProcessorEditor {
public:
    TwoBandEQAudioProcessorEditor (TwoBandEQAudioProcessor&);
    ~TwoBandEQAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    TwoBandEQAudioProcessor& audioProcessor;
    ResponseCurveComponent responseCurve;
    //Слайдер на LOW_GAIN
    juce::Slider lowGainSlider;
    unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowGainAttachment;

    juce::Slider lowFreqSlider;
    unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowFreqAttachment;

    juce::Slider lowQSlider;
    unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowQAttachment;

    //слайдеры на высокие
    juce::Slider highGainSlider;
    unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highGainAttachment;

    juce::Slider highFreqSlider;
    unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highFreqAttachment;

    juce::Slider highQSlider;
    unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highQAttachment;
};
