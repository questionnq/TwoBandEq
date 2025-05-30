#include "ResponseCurveComponent.h"
#include "PluginProcessor.h"
#include <juce_dsp/juce_dsp.h>
using namespace juce;

ResponseCurveComponent::ResponseCurveComponent(TwoBandEQAudioProcessor& p)
    : audioProcessor(p)
{
    startTimerHz(60); // запускаем таймер, 60 кадров в секунду
}

void ResponseCurveComponent::timerCallback()
{
    repaint(); // будет вызывать paint() 60 раз в секунду
}

void ResponseCurveComponent::paint(juce::Graphics& g) {
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawRect(getLocalBounds());

    // 1 для отрисовки: получаем параметры
    auto lowFreq = audioProcessor.apvts.getRawParameterValue("LOW_FREQ")->load();
    auto lowGain = audioProcessor.apvts.getRawParameterValue("LOW_GAIN")->load();
    auto lowQ    = audioProcessor.apvts.getRawParameterValue("LOW_Q")->load();

    auto highFreq = audioProcessor.apvts.getRawParameterValue("HIGH_FREQ")->load();
    auto highGain = audioProcessor.apvts.getRawParameterValue("HIGH_GAIN")->load();
    auto highQ    = audioProcessor.apvts.getRawParameterValue("HIGH_Q")->load();

    //2 сощздаем фильтры
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>,
        dsp::IIR::Coefficients<float>> lowShelfFilter;
    juce::dsp::ProcessorDuplicator<dsp::IIR::Filter<float>,
        dsp::IIR::Coefficients<float>> highShelfFilter;

    lowShelfFilter.state = dsp::IIR::Coefficients<float>::makeLowShelf(
        44100.0, lowFreq, lowQ, Decibels::decibelsToGain(lowGain));
    highShelfFilter.state = dsp::IIR::Coefficients<float>::makeHighShelf(
        44100.0, highFreq, highQ, Decibels::decibelsToGain(highGain));

    if (lowShelfFilter.state == nullptr || highShelfFilter.state == nullptr)
        return;

    //3 создаем путь для кривой АЧХ
    const int responseAreaLeft = 10;
    const int responseAreaRight = getWidth() - 10;
    const int responseAreaTop = 10;
    const int responseAreaBottom = getHeight() - 10;

    Path responseCurve;

    constexpr size_t numPoints = 512; //плавность кривой
    std::vector<double> mags; //список значений усиления в ДБ
    mags.reserve(numPoints);

    for (size_t i = 0; i < numPoints; i++) {
        double normX = static_cast<double>(i) / (numPoints - 1);
        double freq = std::pow(10.0, jmap(normX, 0.0, 1.0, std::log10(20.0), std::log10(20000.0))); //величина логов от 20 гц до 20кгц

        std::complex<double> sample {1.0, 0.0};
        //тут мы вернем амплитудный отклик фильтра при определенной частоте и перемножим оба фильтра
        //чтоб получить сумму усиления
        auto lowMag = lowShelfFilter.state->getMagnitudeForFrequency(freq, 44100.0);
        auto highMag = highShelfFilter.state->getMagnitudeForFrequency(freq, 44100.0);
        auto combinedMag = lowMag * highMag;
        if (combinedMag <= 0.00001)
            combinedMag = 0.00001; // защита от log(0)

        mags.push_back(Decibels::gainToDecibels(combinedMag));
    }

    //функция преобразования децибел в экранную координату Y
    auto mapY = [responseAreaTop, responseAreaBottom](double db) {
        auto normY = jmap(db, -24.0, 24.0, 1.0, 0.0);
        return responseAreaTop + normY * (responseAreaBottom - responseAreaTop);
    };

    //отрисовка пути
    responseCurve.startNewSubPath((float)responseAreaLeft, (float)mapY(mags[0]));
    //и постройка линии целиком
    for (size_t i = 0; i < numPoints; ++i) {
        float x = jmap((float)i, 0.0f, (float)(numPoints - 1), (float)responseAreaLeft, (float)responseAreaRight);
        float y = (float)mapY(mags[i]);
        responseCurve.lineTo({x, y});
        //X равномерно распределяется по всей ширине, а Y = дб на этой частоте.
    }

    g.setColour(Colours::cyan);
    g.strokePath(responseCurve, PathStrokeType(2.0f));


    //блок отрисовки значений частот и делений
    g.setColour(Colours::darkgrey);
    g.setFont(12.0f);

    //дб
    for (int db = -24; db <= 24; db += 6)
    {
        int y = mapY((float)db);
        g.drawHorizontalLine(y, responseAreaLeft, responseAreaRight);

        // подписи уровня
        g.setColour(Colours::white);
        g.drawText(String(db) + " dB", responseAreaLeft + 5, y - 8, 40, 16, Justification::left);
        g.setColour(Colours::darkgrey);
    }

    //частоты
    std::vector<float> freqsToMark = { 20.f, 100.f, 500.f, 1000.f, 5000.f, 10000.f, 20000.f };

    for (float freq : freqsToMark)
    {
        // логарифмическое преобразование
        double normX = (std::log10(freq) - std::log10(20.0)) / (std::log10(20000.0) - std::log10(20.0));
        float x = jmap((float)normX, 0.f, 1.f, (float)responseAreaLeft, (float)responseAreaRight);

        g.drawVerticalLine((int)x, responseAreaTop, responseAreaBottom);

        // подписи частот
        g.setColour(Colours::white);
        String label;
        if (freq >= 1000.0f)
            label = String(freq / 1000.0f, 1) + "k";
        else
            label = String((int)freq);

        int textWidth = 40;
        g.drawText(label,
                   (int)(x - textWidth / 2),
                   responseAreaBottom -5,
                   textWidth, 14,
                   Justification::centred);

        g.setColour(Colours::darkgrey);
    }
}

void ResponseCurveComponent::resized()
{
}

