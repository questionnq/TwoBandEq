#pragma once

#include "JuceHeader.h"
#define JucePlugin_Build_Standalone 1


class TwoBandEQAudioProcessor : public juce::AudioProcessor {
public:
    TwoBandEQAudioProcessor();
    ~TwoBandEQAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //храненилище параметров
    juce::AudioProcessorValueTreeState apvts;

    //метод который создаст список параметров
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> lowShelfLeft, lowShelfRight;
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> highShelfLeft, highShelfRight;

};
