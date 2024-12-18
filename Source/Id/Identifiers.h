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
    inline const juce::Identifier State("State");
    inline const juce::Identifier Theme("Theme");
    inline const juce::Identifier Width("Width");
    inline const juce::Identifier Height("Height");

    inline const juce::Identifier MidiChannelMapper("MidiChannelMapper");
    inline const juce::Identifier MidiChannelMapperList("MidiChannelMapperList");
    inline const juce::Identifier MidiChannelMapperBank("MidiChannelMapperBank");

    inline const juce::Identifier Name("Name");
    inline const juce::Identifier Note("Note");
    inline const juce::Identifier OutputChannel("OutputChannel");

    namespace Str {
        constexpr auto State = "State";
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