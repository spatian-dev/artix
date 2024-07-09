/*
  ==============================================================================

    ChannelMapperList.cpp
    Created: 21 Jun 2024 08:29:56pm
    Author:  Saad Sidqui

  ==============================================================================
*/

#include "Identifiers.h"

namespace Artix::Id {
#define DECLARE_JUCE_ID(name) const juce::Identifier name(#name);

    DECLARE_JUCE_ID(AppState)
    DECLARE_JUCE_ID(Theme)
    DECLARE_JUCE_ID(Width)
    DECLARE_JUCE_ID(Height)

    DECLARE_JUCE_ID(MidiChannelMapper)
    DECLARE_JUCE_ID(MidiChannelMapperBank)

    DECLARE_JUCE_ID(Name)
    DECLARE_JUCE_ID(Note)
    DECLARE_JUCE_ID(InputChannel)
    DECLARE_JUCE_ID(OutputChannel)

#undef DECLARE_JUCE_ID
}