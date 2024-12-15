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
#define DEFINE_JUCE_ID(name) extern const juce::Identifier name; namespace Str { constexpr auto name = #name;}
	
	DEFINE_JUCE_ID(AppState)
	DEFINE_JUCE_ID(Theme)
	DEFINE_JUCE_ID(Height)

	DEFINE_JUCE_ID(MidiChannelMapper)
	DEFINE_JUCE_ID(MidiChannelMapperBank)

	DEFINE_JUCE_ID(Name)
	DEFINE_JUCE_ID(Note)
	DEFINE_JUCE_ID(InputChannel)
	DEFINE_JUCE_ID(OutputChannel)

#undef DEFINE_JUCE_ID
}