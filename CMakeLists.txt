cmake_minimum_required(VERSION 3.15)

project(TwoBandEQ VERSION 0.1.0)

set(JUCE_DIR "C:/Program Files (x86)/dev/JUCE")

add_subdirectory("${JUCE_DIR}" "${CMAKE_BINARY_DIR}/juce_build")

juce_add_plugin(TwoBandEQ
    COMPANY_NAME "questionnq"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE TBEQ
    FORMATS VST3 Standalone
    PRODUCT_NAME "TwoBandEQ"
)

target_sources(TwoBandEQ PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/JuceHeader.h Source/ResponseCurveComponent.cpp Source/ResponseCurveComponent.h

)

target_link_libraries(TwoBandEQ PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_basics
)

juce_generate_juce_header(TwoBandEQ)

target_compile_definitions(TwoBandEQ
        PRIVATE
        JucePlugin_Build_Standalone=1
)

juce_add_plugin(...)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
)

