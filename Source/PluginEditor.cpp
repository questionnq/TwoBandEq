#include "PluginEditor.h"
#include "PluginProcessor.h"
#include <juce_dsp/juce_dsp.h>
using namespace std;
using namespace juce;

TwoBandEQAudioProcessorEditor::TwoBandEQAudioProcessorEditor (TwoBandEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), responseCurve(p){
    setSize (600, 350);

    addAndMakeVisible(responseCurve);

    //СЛАЙДЕРЫ НА НИЗА
    // Слайдер на LOW_GAIN
    lowGainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(lowGainSlider);

    //Привязка слайдера к LOW_GAIN
    lowGainAttachment = make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "LOW_GAIN", lowGainSlider);

    // Слайдер на LOW_FREQ
    lowFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(lowFreqSlider);

    //привязка слайдера к LOW_FREQ
    lowFreqAttachment = make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "LOW_FREQ", lowFreqSlider);

    //Слайдер на LOW_Q
    lowQSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lowQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(lowQSlider);

    //привязка слайдера к LOW_Q
    lowQAttachment = make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "LOW_Q", lowQSlider);

    //СЛАЙДЕРЫ НА ВЕРХА
    highGainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    highGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(highGainSlider);
    highGainAttachment = make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "HIGH_GAIN", highGainSlider);

    highFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    highFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(highFreqSlider);
    highFreqAttachment = make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "HIGH_FREQ", highFreqSlider);

    highQSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    highQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(highQSlider);
    highQAttachment = make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "HIGH_Q", highQSlider);


}


void TwoBandEQAudioProcessorEditor::paint (juce::Graphics& g) {
    g.fillAll (juce::Colours::transparentBlack);

    g.setColour (juce::Colours::white);
    g.setFont (16.0f);

    g.setFont (14.0f);

    // Метка "LOW" над первой группой
    g.drawFittedText ("LOW", 20, 40, 3 * 80 + 2 * 10, 20, juce::Justification::centred, 1);

    // Метка "HIGH" над второй группой
    g.drawFittedText ("HIGH", 320, 40, 3 * 80 + 2 * 10, 20, juce::Justification::centred, 1);

    // Подписи Low
    g.drawFittedText ("Low Gain", lowGainSlider.getX(), lowGainSlider.getY() - 18,
                      lowGainSlider.getWidth(), 16, juce::Justification::centred, 1);
    g.drawFittedText ("Low Freq", lowFreqSlider.getX(), lowFreqSlider.getY() - 18,
                      lowFreqSlider.getWidth(), 16, juce::Justification::centred, 1);
    g.drawFittedText ("Low Q", lowQSlider.getX(), lowQSlider.getY() - 18,
                      lowQSlider.getWidth(), 16, juce::Justification::centred, 1);

    // Подписи High
    g.drawFittedText ("High Gain", highGainSlider.getX(), highGainSlider.getY() - 18,
                      highGainSlider.getWidth(), 16, juce::Justification::centred, 1);
    g.drawFittedText ("High Freq", highFreqSlider.getX(), highFreqSlider.getY() - 18,
                      highFreqSlider.getWidth(), 16, juce::Justification::centred, 1);
    g.drawFittedText ("High Q", highQSlider.getX(), highQSlider.getY() - 18,
                      highQSlider.getWidth(), 16, juce::Justification::centred, 1);
}


void TwoBandEQAudioProcessorEditor::resized()
{
    responseCurve.setBounds(120, 20, getWidth() - 240, getHeight() - 40);

    const int sliderWidth = 80;
    const int sliderHeight = 80;
    const int verticalSpacing = 20;

    const int leftX = 30;
    const int rightX = getWidth() - sliderWidth - 30;

    const int topY = 60;

    // LOW фильтр — слева, вертикально
    lowGainSlider.setBounds(leftX, topY, sliderWidth, sliderHeight);
    lowFreqSlider.setBounds(leftX, topY + sliderHeight + verticalSpacing, sliderWidth, sliderHeight);
    lowQSlider.setBounds(leftX, topY + 2 * (sliderHeight + verticalSpacing), sliderWidth, sliderHeight);

    // HIGH фильтр — справа, вертикально
    highGainSlider.setBounds(rightX, topY, sliderWidth, sliderHeight);
    highFreqSlider.setBounds(rightX, topY + sliderHeight + verticalSpacing, sliderWidth, sliderHeight);
    highQSlider.setBounds(rightX, topY + 2 * (sliderHeight + verticalSpacing), sliderWidth, sliderHeight);
}
