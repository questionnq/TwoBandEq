#pragma once
#ifndef RESPONCECURVECOMPONENT_H
#define RESPONCECURVECOMPONENT_H
#include <JuceHeader.h>
#include "PluginProcessor.h"
class ResponseCurveComponent : public juce::Component, private juce::Timer {
public:
    ResponseCurveComponent(TwoBandEQAudioProcessor&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    TwoBandEQAudioProcessor& audioProcessor;
    juce::Image background;
};
#endif //RESPONCECURVECOMPONENT_H
