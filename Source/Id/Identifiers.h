/*
  ==============================================================================

    Identifiers.h
    Created: 13 Jul 2024 3:25:31pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Artix::Id {
    extern const juce::Identifier AppState;
    extern const juce::Identifier Theme;
    extern const juce::Identifier Height;
    extern const juce::Identifier MidiChannelMapper;
    extern const juce::Identifier MidiChannelMapperBank;
    extern const juce::Identifier Name;
    extern const juce::Identifier Note;
    extern const juce::Identifier InputChannel;
    extern const juce::Identifier OutputChannel;

    namespace Str {
        constexpr auto AppState = "AppState";
        constexpr auto Theme = "Theme";
        constexpr auto Height = "Height";
        constexpr auto MidiChannelMapper = "MidiChannelMapper";
        constexpr auto MidiChannelMapperBank = "MidiChannelMapperBank";
        constexpr auto Name = "Name";
        constexpr auto Note = "Note";
        constexpr auto InputChannel = "InputChannel";
        constexpr auto OutputChannel = "OutputChannel";
    }
}