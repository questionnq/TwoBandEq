#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;
using namespace std;

TwoBandEQAudioProcessor::TwoBandEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
    //ниже находится apvts, в который мы
    //передаем ссылку на плагин, nullptr, имя древа и список парметров
    apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

TwoBandEQAudioProcessor::~TwoBandEQAudioProcessor() {}

void TwoBandEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();

    lowShelfLeft.prepare(spec);
    lowShelfRight.prepare(spec);
    highShelfLeft.prepare(spec);
    highShelfRight.prepare(spec);
}
void TwoBandEQAudioProcessor::releaseResources() {}

void TwoBandEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    //низкие частоты:
    float gainDb = apvts.getRawParameterValue("LOW_GAIN")->load();
    float freq = apvts.getRawParameterValue("LOW_FREQ")->load();
    float q    = apvts.getRawParameterValue("LOW_Q")->load();

    auto lowShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        getSampleRate(),
        freq,
        q,
        juce::Decibels::decibelsToGain(gainDb));

    *lowShelfLeft.state = *lowShelfCoefficients;
    *lowShelfRight.state = *lowShelfCoefficients;

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    lowShelfLeft.process(leftContext);
    lowShelfRight.process(rightContext);

    //высокие частоты:
    float highGain = apvts.getRawParameterValue("HIGH_GAIN")->load();
    float highFreq = apvts.getRawParameterValue("HIGH_FREQ")->load();
    float highQ    = apvts.getRawParameterValue("HIGH_Q")->load();

    auto highShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
    getSampleRate(), highFreq, highQ, juce::Decibels::decibelsToGain(highGain));

    *highShelfLeft.state = *highShelfCoeffs;
    *highShelfRight.state = *highShelfCoeffs;

    highShelfLeft.process(leftContext);
    highShelfRight.process(rightContext);
}

juce::AudioProcessorEditor* TwoBandEQAudioProcessor::createEditor() {
    return new TwoBandEQAudioProcessorEditor (*this);
}
bool TwoBandEQAudioProcessor::hasEditor() const { return true; }

const juce::String TwoBandEQAudioProcessor::getName() const { return JucePlugin_Name; }
bool TwoBandEQAudioProcessor::acceptsMidi() const { return false; }
bool TwoBandEQAudioProcessor::producesMidi() const { return false; }
bool TwoBandEQAudioProcessor::isMidiEffect() const { return false; }
double TwoBandEQAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TwoBandEQAudioProcessor::getNumPrograms() { return 1; }
int TwoBandEQAudioProcessor::getCurrentProgram() { return 0; }
void TwoBandEQAudioProcessor::setCurrentProgram (int) {}
const juce::String TwoBandEQAudioProcessor::getProgramName (int) { return {}; }
void TwoBandEQAudioProcessor::changeProgramName (int, const juce::String&) {}

void TwoBandEQAudioProcessor::getStateInformation (juce::MemoryBlock&) {}
void TwoBandEQAudioProcessor::setStateInformation (const void*, int) {}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new TwoBandEQAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TwoBandEQAudioProcessor::createParameters() {
    vector<unique_ptr<juce::RangedAudioParameter>> params;

    //создаем параметр LOW_GAIN - ID(машинный), Name(отображаемый),
    //Диапазон и диапазон по умолчанию
    params.push_back(make_unique<juce::AudioParameterFloat>(
        "LOW_GAIN", "Low Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f), 0.0f));

    //параметр частоты среза
    params.push_back(make_unique<juce::AudioParameterFloat>(
        "LOW_FREQ", "Low Freq", juce::NormalisableRange<float>(20.0f,
            1000.0f, 1.0f), 200.0f));

    //ширина среза
    params.push_back(make_unique<juce::AudioParameterFloat>(
        "LOW_Q", "Low Q", juce::NormalisableRange<float>(0.1f, 10.0f,
            0.01f), 0.707f));

    //все тоже самое для высоких частот
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
    "HIGH_GAIN", "High Gain", juce::NormalisableRange<float>(-24.0f, 24.0f), 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HIGH_FREQ", "High Freq", juce::NormalisableRange<float>(1000.0f, 20000.0f, 1.0f), 8000.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HIGH_Q", "High Q", juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 0.707f));

    return { params.begin(), params.end() };
}
