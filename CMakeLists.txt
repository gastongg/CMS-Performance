cmake_minimum_required(VERSION 3.22)

project(CMS_Performance_DJ VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE
add_subdirectory("C:/JUCE" JUCE)

# Aplicación CMS Performance DJ
juce_add_gui_app(CMS_Performance_DJ
    PRODUCT_NAME "CMS Performance DJ"
    COMPANY_NAME "Chocomix"
    BUNDLE_ID "com.chocomix.cmsperformancedj"
    VERSION "0.1.0"
)

target_sources(CMS_Performance_DJ
    PRIVATE
        Source/Main.cpp
)

# Generar JuceHeader.h
juce_generate_juce_header(CMS_Performance_DJ)

target_compile_definitions(CMS_Performance_DJ
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

target_link_libraries(CMS_Performance_DJ
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_dsp
        juce::juce_gui_extra
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)